# albumhelpers.py
#
# A series of funcitons to build the albums datastructure

import sys, json, logging, time, os, urllib2

# Friends

def get_friends(q_wrap, uid):
    '''returns a list of uids and names of uid's friends'''

    q = ''.join(['SELECT uid, name FROM user WHERE uid IN ',
                 '(SELECT uid2 FROM friend WHERE uid1 = %(name)s) ',
                 'OR uid=%(name)s']) % {"name":uid}
    people = q_wrap(q)
    people.sort()

    me = dict(uid=uid,name="Myself")
    people.insert(0,me)
    return people

def get_friend(q_wrap, uid):
    '''get the name of a specific uid'''
    q = 'SELECT name FROM profile WHERE id=%s'
    res = q_wrap(q % uid)
    if res:
        return res[0]['name']
    else:
        return 'uid_%s' % uid # could not find real name

def get_friend_name(q_wrap, friends, uid):
    '''returns a person's name.  Adds the name if it doesn't exist.'''
    if uid == None or uid == '':
        # this should never happen... but it did once
        return 'Unknown'
    if uid not in friends:
        friends[uid] = get_friend(q_wrap, uid)
    return friends[uid]

# Albums/Pictures

def get_tagged_albums(q_wrap, uid, albums):
    '''return info from all albums in which the uid was tagged'''

    # limit 5000 problem for this query
    q = ''.join(['SELECT aid FROM photo WHERE pid IN (SELECT ',
                 'pid FROM photo_tag WHERE subject="%s")']) % uid
    album_ids = q_wrap(q)
    album_ids_n = tuple(set('%s' % x['aid'] for x in album_ids))
    album_ids = tuple(set('"%s"' % x['aid'] for x in album_ids))

    # 50 seems like a good number of album_ids to retrieve at a time...
    q = ''.join(['SELECT aid, owner, name, modified, description, ',
                 'location, object_id FROM album WHERE aid IN (%s)'])
    for i in range(len(album_ids) / 50 + 1):
        aids = ','.join(album_ids[i*50:(i+1)*50])
        for item in q_wrap(q % aids):
            item['photos'] = {}
            albums[item['aid']] = item

    # due to permissions some albums might not have info
    for aid in album_ids_n:
        if aid not in albums.keys():
            fill_empty_album(q_wrap, aid, albums)

def fill_empty_album(q_wrap, aid, albums):
    album = {}
    album['aid'] = aid
    album['owner'] = ''
    album['name'] = 'Unknown_%s' % aid
    album['modified'] = '0'
    album['description'] = ''
    album['location'] = ''
    album['photos'] = {}
    # no object id
    albums[aid] = album

def get_uploaded_albums(q_wrap, uid, albums):
    '''return info from all albums uploaded by uid'''
    q = ''.join(['SELECT aid, owner, name, modified, description, ',
                 'location, object_id FROM album WHERE ',
                 'owner="%s"']) % uid

    for item in q_wrap(q):
        item['photos'] = {}
        albums[item['aid']] = item

def get_tagged_pictures(q_wrap, uid, albums):
    '''add all pictures where the user is tagged'''
    # limit 5000 problem for this query
    q = ''.join(['SELECT pid, aid, src_big, caption, created, object_id ',
                 'FROM photo WHERE pid IN (SELECT pid FROM photo_tag ',
                 'WHERE subject="%s")']) % uid

    for photo in q_wrap(q):
        albums[photo['aid']]['photos'][photo['pid']] = photo


def get_tagged_album_pictures(q_wrap, uid, albums):
    '''add full albums where the user is tagged'''

    album_ids = tuple(set('"%s"' % x for x in albums))
    # query in groups of 25 (limit 5000 each)
    q = ''.join(['SELECT pid, aid, src_big, caption, ',
                 'created, object_id FROM photo WHERE aid IN (%s)'])
    for i in range(len(album_ids) / 25 + 1):
        aids = ','.join(album_ids[i * 25:(i+1) * 25])

        for photo in q_wrap(q % aids):
            albums[photo['aid']]['photos'][photo['pid']] = photo

def get_user_album_pictures(q_wrap, uid, albums):
    '''all pictures in albums uploaded by the user'''
    q = ''.join(['SELECT pid, aid, src_big, caption, created, ',
                 'object_id FROM photo WHERE aid IN (SELECT aid FROM ',
                 'album WHERE owner="%s")']) % uid

    for photo in q_wrap(q):
        albums[photo['aid']]['photos'][photo['pid']] = photo


def get_album_comments(q_wrap, album, friends):
    '''get the comments for a single album (including the pictures)'''

    q = ''.join(['SELECT object_id, fromid, time, text FROM comment ',
                 'WHERE object_id in (%s)'])

    o2pid = {} # translate object_id to pid
    oids = [] # hold object_ids to lookup

    album_comments = []

    for photo in album['photos'].values():
        o2pid[photo['object_id']] = photo['pid']
        oids.append('"%s"' % photo['object_id'])

    if 'object_id' in album: oids.append('"%s"' % album['object_id'])

    # load all comments for album and its photos
    for item in q_wrap(q % ','.join(oids)):
        oid = item['object_id']
        # add the friend's name
        item['fromname'] = get_friend_name(q_wrap, friends, item['fromid'])
        if oid in o2pid: # photo comment
            clist = album['photos'][o2pid[oid]].setdefault('comments',
                                                           [])
            clist.append(item)
        else: # album comment
            album_comments.append(item)

    album['comments'] = album_comments

    # load tags in each photo
    q = ''.join(['SELECT pid, text, xcoord, ycoord FROM ',
                 'photo_tag WHERE pid IN(%s)'])
    pids = ['"%s"' % x for x in album['photos'].keys()]
    for item in q_wrap(q % ','.join(pids)):
        tag_list = album['photos'][item['pid']].setdefault('tags', [])
        tag_list.append(item)

def add_photo_paths(album):
    '''set path info in album dictionary'''
    for photo in album['photos'].items():
        #photo[1]['path'] = os.path.join(album['folder'], '%s.jpg' % photo[0])
        photo[1]['path'] = '%s.jpg' % photo[0]

# Save commands

def save_albums_dict(albums, friends, path):
    '''save the albums and friends dictonaries to json files'''
    ts = time.strftime("%y-%m-%d_%H-%M-%S")

    for album in albums.values():
        filename = os.path.join(path, album['folder'], 'pg_%s.json' % ts)
        alfilename = os.path.join(path, album['folder'], 'album.json')
        htmlfilename = os.path.join(path, album['folder'], 'viewer.html')
        try:
            db_file = open(filename, "w")
            db_file.write("var al = ");
            json.dump(album, db_file)
            db_file.write(";\n")
            db_file.close()
            import shutil
            shutil.copy(filename, alfilename)
            shutil.copy(os.path.join('dep', 'viewer.html'), htmlfilename)
        except Exception, e:
            logging.exception('Saving JSON Failed: %s', filename)

def download_pic(photo, filename):
    '''downloads a picture (retries 10 times before failure)'''
    max_retries = 10
    retries = 0

    picout = open(filename, 'wb')
    handler = urllib2.Request(photo['src_big'])
    retry = True

    while retry:
        try:
            logging.debug('downloading:%s' % photo['src_big'])
            data = urllib2.urlopen(handler)
            retry = False
        except Exception, e:
            if retries < max_retries:
                retries += 1
                logging.debug('retrying download %s' % filename)
                # sleep longer and longer between retries
                time.sleep(retries * 2)
            else:
                # skip on 404 error: Issue 13
                logging.exception('Could not download %s' % filename)
                picout.close()
                os.remove(filename)
                return

    picout.write(data.read())
    picout.close()
    os.utime(filename, (int(photo['created']),) * 2)
