#!/usr/bin/env python
# coding: utf-8
# Copyright 2011 Facebook, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License. You may obtain
# a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.


import os
# dummy config to enable registering django template filters
os.environ[u'DJANGO_SETTINGS_MODULE'] = u'conf'

from google.appengine.dist import use_library
use_library('django', '1.2')

from django.template.defaultfilters import register
from django.utils import simplejson as json
from functools import wraps
from google.appengine.api import urlfetch, taskqueue
from google.appengine.ext import db, webapp
from google.appengine.ext.webapp import util, template
from google.appengine.runtime import DeadlineExceededError
from random import randrange
from uuid import uuid4
import Cookie
import base64
import cgi
import conf
import datetime
import hashlib
import hmac
import logging
import time
import traceback
import urllib


def htmlescape(text):
    """Escape text for use as HTML"""
    return cgi.escape(
        text, True).replace("'", '&#39;').encode('ascii', 'xmlcharrefreplace')


@register.filter(name=u'get_name')
def get_name(dic, index):
    """Django template filter to render name"""
    return dic[index].name


@register.filter(name=u'get_picture')
def get_picture(dic, index):
    """Django template filter to render picture"""
    return dic[index].picture


def select_random(lst, limit):
    """Select a limited set of random non Falsy values from a list"""
    final = []
    size = len(lst)
    while limit and size:
        index = randrange(min(limit, size))
        size = size - 1
        elem = lst[index]
        lst[index] = lst[size]
        if elem:
            limit = limit - 1
            final.append(elem)
    return final


_USER_FIELDS = u'name,email,picture,friends'
class User(db.Model):
    user_id = db.StringProperty(required=True)
    access_token = db.StringProperty(required=True)
    name = db.StringProperty(required=True)
    picture = db.StringProperty(required=True)
    email = db.StringProperty()
    friends = db.StringListProperty()
    dirty = db.BooleanProperty()

    def refresh_data(self):
        """Refresh this user's data using the Facebook Graph API"""
        me = Facebook().api(u'/me',
            {u'fields': _USER_FIELDS, u'access_token': self.access_token})
        self.dirty = False
        self.name = me[u'name']
        self.email = me.get(u'email')
        self.picture = me[u'picture']
        self.friends = [user[u'id'] for user in me[u'friends'][u'data']]
        return self.put()


class Run(db.Model):
    user_id = db.StringProperty(required=True)
    location = db.StringProperty(required=True)
    distance = db.FloatProperty(required=True)
    date = db.DateProperty(required=True)

    @staticmethod
    def find_by_user_ids(user_ids, limit=50):
        if user_ids:
            return Run.gql(u'WHERE user_id IN :1', user_ids).fetch(limit)
        else:
            return []

    @property
    def pretty_distance(self):
        return u'%.2f' % self.distance


class RunException(Exception):
    pass


class FacebookApiError(Exception):
    def __init__(self, result):
        self.result = result

    def __str__(self):
        return self.__class__.__name__ + ': ' + json.dumps(self.result)


class Facebook(object):
    """Wraps the Facebook specific logic"""
    def __init__(self, app_id=conf.FACEBOOK_APP_ID,
            app_secret=conf.FACEBOOK_APP_SECRET):
        self.app_id = app_id
        self.app_secret = app_secret
        self.user_id = None
        self.access_token = None
        self.signed_request = {}

    def api(self, path, params=None, method=u'GET', domain=u'graph'):
        """Make API calls"""
        if not params:
            params = {}
        params[u'method'] = method
        if u'access_token' not in params and self.access_token:
            params[u'access_token'] = self.access_token
        result = json.loads(urlfetch.fetch(
            url=u'https://' + domain + u'.facebook.com' + path,
            payload=urllib.urlencode(params),
            method=urlfetch.POST,
            headers={
                u'Content-Type': u'application/x-www-form-urlencoded'})
            .content)
        if isinstance(result, dict) and u'error' in result:
            raise FacebookApiError(result)
        return result

    def load_signed_request(self, signed_request):
        """Load the user state from a signed_request value"""
        try:
            sig, payload = signed_request.split(u'.', 1)
            sig = self.base64_url_decode(sig)
            data = json.loads(self.base64_url_decode(payload))

            expected_sig = hmac.new(
                self.app_secret, msg=payload, digestmod=hashlib.sha256).digest()

            # allow the signed_request to function for upto 1 day
            if sig == expected_sig and \
                    data[u'issued_at'] > (time.time() - 86400):
                self.signed_request = data
                self.user_id = data.get(u'user_id')
                self.access_token = data.get(u'oauth_token')
        except ValueError, ex:
            pass # ignore if can't split on dot

    @property
    def user_cookie(self):
        """Generate a signed_request value based on current state"""
        if not self.user_id:
            return
        payload = self.base64_url_encode(json.dumps({
            u'user_id': self.user_id,
            u'issued_at': str(int(time.time())),
        }))
        sig = self.base64_url_encode(hmac.new(
            self.app_secret, msg=payload, digestmod=hashlib.sha256).digest())
        return sig + '.' + payload

    @staticmethod
    def base64_url_decode(data):
        data = data.encode(u'ascii')
        data += '=' * (4 - (len(data) % 4))
        return base64.urlsafe_b64decode(data)

    @staticmethod
    def base64_url_encode(data):
        return base64.urlsafe_b64encode(data).rstrip('=')


class CsrfException(Exception):
    pass


class BaseHandler(webapp.RequestHandler):
    facebook = None
    user = None
    csrf_protect = True

    def initialize(self, request, response):
        """General initialization for every request"""
        super(BaseHandler, self).initialize(request, response)

        try:
            self.init_facebook()
            self.init_csrf()
            self.response.headers[u'P3P'] = u'CP=HONK'  # iframe cookies in IE
        except Exception, ex:
            self.log_exception(ex)
            raise

    def handle_exception(self, ex, debug_mode):
        """Invoked for unhandled exceptions by webapp"""
        self.log_exception(ex)
        self.render(u'error',
            trace=traceback.format_exc(), debug_mode=debug_mode)

    def log_exception(self, ex):
        """Internal logging handler to reduce some App Engine noise in errors"""
        msg = ((str(ex) or ex.__class__.__name__) +
                u': \n' + traceback.format_exc())
        if isinstance(ex, urlfetch.DownloadError) or \
           isinstance(ex, DeadlineExceededError) or \
           isinstance(ex, CsrfException) or \
           isinstance(ex, taskqueue.TransientError):
            logging.warn(msg)
        else:
            logging.error(msg)

    def set_cookie(self, name, value, expires=None):
        """Set a cookie"""
        if value is None:
            value = 'deleted'
            expires = datetime.timedelta(minutes=-50000)
        jar = Cookie.SimpleCookie()
        jar[name] = value
        jar[name]['path'] = u'/'
        if expires:
            if isinstance(expires, datetime.timedelta):
                expires = datetime.datetime.now() + expires
            if isinstance(expires, datetime.datetime):
                expires = expires.strftime('%a, %d %b %Y %H:%M:%S')
            jar[name]['expires'] = expires
        self.response.headers.add_header(*jar.output().split(u': ', 1))

    def render(self, name, **data):
        """Render a template"""
        if not data:
            data = {}
        data[u'js_conf'] = json.dumps({
            u'appId': conf.FACEBOOK_APP_ID,
            u'canvasName': conf.FACEBOOK_CANVAS_NAME,
            u'userIdOnServer': self.user.user_id if self.user else None,
        })
        data[u'logged_in_user'] = self.user
        data[u'message'] = self.get_message()
        data[u'csrf_token'] = self.csrf_token
        data[u'canvas_name'] = conf.FACEBOOK_CANVAS_NAME
        self.response.out.write(template.render(
            os.path.join(
                os.path.dirname(__file__), 'templates', name + '.html'),
            data))

    def init_facebook(self):
        """Sets up the request specific Facebook and User instance"""
        facebook = Facebook()
        user = None

        # initial facebook request comes in as a POST with a signed_request
        if u'signed_request' in self.request.POST:
            facebook.load_signed_request(self.request.get('signed_request'))
            # we reset the method to GET because a request from facebook with a
            # signed_request uses POST for security reasons, despite it
            # actually being a GET. in webapp causes loss of request.POST data.
            self.request.method = u'GET'
            self.set_cookie(
                'u', facebook.user_cookie, datetime.timedelta(minutes=1440))
        elif 'u' in self.request.cookies:
            facebook.load_signed_request(self.request.cookies.get('u'))

        # try to load or create a user object
        if facebook.user_id:
            user = User.get_by_key_name(facebook.user_id)
            if user:
                # update stored access_token
                if facebook.access_token and \
                        facebook.access_token != user.access_token:
                    user.access_token = facebook.access_token
                    user.put()
                # refresh data if we failed in doing so after a realtime ping
                if user.dirty:
                    user.refresh_data()
                # restore stored access_token if necessary
                if not facebook.access_token:
                    facebook.access_token = user.access_token

            if not user and facebook.access_token:
                me = facebook.api(u'/me', {u'fields': _USER_FIELDS})
                try:
                    friends = [user[u'id'] for user in me[u'friends'][u'data']]
                    user = User(key_name=facebook.user_id,
                        user_id=facebook.user_id, friends=friends,
                        access_token=facebook.access_token, name=me[u'name'],
                        email=me.get(u'email'), picture=me[u'picture'])
                    user.put()
                except KeyError, ex:
                    pass # ignore if can't get the minimum fields

        self.facebook = facebook
        self.user = user

    def init_csrf(self):
        """Issue and handle CSRF token as necessary"""
        self.csrf_token = self.request.cookies.get(u'c')
        if not self.csrf_token:
            self.csrf_token = str(uuid4())[:8]
            self.set_cookie('c', self.csrf_token)
        if self.request.method == u'POST' and self.csrf_protect and \
                self.csrf_token != self.request.POST.get(u'_csrf_token'):
            raise CsrfException(u'Missing or invalid CSRF token.')

    def set_message(self, **obj):
        """Simple message support"""
        self.set_cookie('m', base64.b64encode(json.dumps(obj)) if obj else None)

    def get_message(self):
        """Get and clear the current message"""
        message = self.request.cookies.get(u'm')
        if message:
            self.set_message()  # clear the current cookie
            return json.loads(base64.b64decode(message))


def user_required(fn):
    """Decorator to ensure a user is present"""
    @wraps(fn)
    def wrapper(*args, **kwargs):
        handler = args[0]
        if handler.user:
            return fn(*args, **kwargs)
        handler.redirect(u'/')
    return wrapper


class RecentRunsHandler(BaseHandler):
    """Show recent runs for the user and friends"""
    def get(self):
        if self.user:
            friends = {}
            for friend in select_random(
                    User.get_by_key_name(self.user.friends), 30):
                friends[friend.user_id] = friend

            self.render(u'runs',
                friends=friends,
                user_recent_runs=Run.find_by_user_ids(
                    [self.user.user_id], limit=5),
                friends_runs=Run.find_by_user_ids(friends.keys()),
            )
        else:
            self.render(u'welcome')


class UserRunsHandler(BaseHandler):
    """Show a specific user's runs, ensure friendship with the logged in user"""
    @user_required
    def get(self, user_id):
        if self.user.friends.count(user_id) or self.user.user_id == user_id:
            user = User.get_by_key_name(user_id)
            if not user:
                self.set_message(type=u'error',
                    content=u'That user does not use Run with Friends.')
                self.redirect(u'/')
                return

            self.render(u'user',
                user=user,
                runs=Run.find_by_user_ids([user_id]),
            )
        else:
            self.set_message(type=u'error',
                content=u'You are not allowed to see that.')
            self.redirect(u'/')


class RunHandler(BaseHandler):
    """Add a run"""
    @user_required
    def post(self):
        try:
            location = self.request.POST[u'location'].strip()
            if not location:
                raise RunException(u'Please specify a location.')

            distance = float(self.request.POST[u'distance'].strip())
            if distance < 0:
                raise RunException(u'Invalid distance.')

            date_year = int(self.request.POST[u'date_year'].strip())
            date_month = int(self.request.POST[u'date_month'].strip())
            date_day = int(self.request.POST[u'date_day'].strip())
            if date_year < 0 or date_month < 0 or date_day < 0:
                raise RunException(u'Invalid date.')
            date = datetime.date(date_year, date_month, date_day)

            run = Run(
                user_id=self.user.user_id,
                location=location,
                distance=distance,
                date=date,
            )
            run.put()

            title = run.pretty_distance + u' miles @' + location
            publish = u'<a onclick=\'publishRun(' + \
                    json.dumps(htmlescape(title)) + u')\'>Post to facebook.</a>'
            self.set_message(type=u'success',
                content=u'Added your run. ' + publish)
        except RunException, e:
            self.set_message(type=u'error', content=unicode(e))
        except KeyError:
            self.set_message(type=u'error',
                content=u'Please specify location, distance & date.')
        except ValueError:
            self.set_message(type=u'error',
                content=u'Please specify a valid distance & date.')
        except Exception, e:
            self.set_message(type=u'error',
                content=u'Unknown error occured. (' + unicode(e) + u')')
        self.redirect(u'/')


class RealtimeHandler(BaseHandler):
    """Handles Facebook Real-time API interactions"""
    csrf_protect = False

    def get(self):
        if (self.request.GET.get(u'setup') == u'1' and
            self.user and conf.ADMIN_USER_IDS.count(self.user.user_id)):
            self.setup_subscription()
            self.set_message(type=u'success',
                content=u'Successfully setup Real-time subscription.')
        elif (self.request.GET.get(u'hub.mode') == u'subscribe' and
              self.request.GET.get(u'hub.verify_token') ==
                  conf.FACEBOOK_REALTIME_VERIFY_TOKEN):
            self.response.out.write(self.request.GET.get(u'hub.challenge'))
            logging.info(
                u'Successful Real-time subscription confirmation ping.')
            return
        else:
            self.set_message(type=u'error',
                content=u'You are not allowed to do that.')
        self.redirect(u'/')

    def post(self):
        body = self.request.body
        if self.request.headers[u'X-Hub-Signature'] != (u'sha1=' + hmac.new(
            self.facebook.app_secret,
            msg=body,
            digestmod=hashlib.sha1).hexdigest()):
            logging.error(
                u'Real-time signature check failed: ' + unicode(self.request))
            return
        data = json.loads(body)

        if data[u'object'] == u'user':
            for entry in data[u'entry']:
                taskqueue.add(url=u'/task/refresh-user/' + entry[u'id'])
                logging.info('Added task to queue to refresh user data.')
        else:
            logging.warn(u'Unhandled Real-time ping: ' + body)

    def setup_subscription(self):
        path = u'/' + conf.FACEBOOK_APP_ID + u'/subscriptions'
        params = {
            u'access_token': conf.FACEBOOK_APP_ID + u'|' +
                             conf.FACEBOOK_APP_SECRET,
            u'object': u'user',
            u'fields': _USER_FIELDS,
            u'callback_url': conf.EXTERNAL_HREF + u'realtime',
            u'verify_token': conf.FACEBOOK_REALTIME_VERIFY_TOKEN,
        }
        response = self.facebook.api(path, params, u'POST')
        logging.info(u'Real-time setup API call response: ' + unicode(response))


class RefreshUserHandler(BaseHandler):
    """Used as an App Engine Task to refresh a single user's data if possible"""
    csrf_protect = False

    def post(self, user_id):
        logging.info('Refreshing user data for ' + user_id)
        user = User.get_by_key_name(user_id)
        if not user:
            return
        try:
            user.refresh_data()
        except FacebookApiError:
            user.dirty = True
            user.put()


def main():
    routes = [
        (r'/', RecentRunsHandler),
        (r'/user/(.*)', UserRunsHandler),
        (r'/run', RunHandler),
        (r'/realtime', RealtimeHandler),

        (r'/task/refresh-user/(.*)', RefreshUserHandler),
    ]
    application = webapp.WSGIApplication(routes,
        debug=os.environ.get('SERVER_SOFTWARE', '').startswith('Dev'))
    util.run_wsgi_app(application)


if __name__ == u'__main__':
    main()
