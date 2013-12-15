import os, re, sys, time
from threading import Thread
import multiprocessing
import albumhelpers
import logging

class FBDownloader(Thread):
    REPLACE_RE = re.compile(r'\*|"|\'|:|<|>|\?|\\|/|\|,|\|| ')

    def __init__ (self, photos_path, uid, friends,
                        full_albums, user_albums, extras, graph,
                        update_callback, force_exit_callback):
        Thread.__init__(self)
        self.photos_path = photos_path
        self.uid = uid
        self.friends = friends
        self.graph = graph
        # options
        self.full_albums = full_albums
        self.user_albums = user_albums
        self.extras = extras
        # callback functions
        self.update = update_callback
        self.force_exit = force_exit_callback

        self._thread_terminated = False
        self.po = None # sub processes
        self.index = self.total = 0
        self.albums = {}        # hold all pic data

    # terminate the thread if need be
    def exit_if_terminated(self):
        if self._thread_terminated:
            logging.info('GUI terminating download thread')
            if self.po:
               self.po.terminate() # stop workers
            sys.exit() # raise SystemExit exception to terminate run()

    # queries fail a lot, retry them many times
    def query_wrapper(self, q):
        max_retries = 10
        retries = 0
        while True:
            # check if thread should be terminated
            self.exit_if_terminated()
            try:
                return self.graph.fql(q)
            except Exception, e:
                if retries < max_retries:
                    logging.info('retrying function: %d' % retries)
                    logging.info('Reason: %s' % e)
                    retries += 1
                    # sleep longer and longer between retries
                    time.sleep(retries * 2)
                else:
                    raise

    # functions to get FQL data

    def get_albums(self):
        albumhelpers.get_tagged_albums(self.query_wrapper, self.uid, self.albums)
        if self.user_albums:
            albumhelpers.get_uploaded_albums(self.query_wrapper, self.uid, self.albums)

    def get_pictures(self):
        if self.full_albums:
            albumhelpers.get_tagged_album_pictures(self.query_wrapper, self.uid, self.albums)
        else:
            albumhelpers.get_tagged_pictures(self.query_wrapper, self.uid, self.albums)

        if self.user_albums:
            albumhelpers.get_user_album_pictures(self.query_wrapper, self.uid, self.albums)


    # process and save albums

    def save_albums(self):
        # set album folder names and get extra info

        for album in self.albums.values():
            username = albumhelpers.get_friend_name(self.query_wrapper,
                                                    self.friends,
                                                    album['owner'])
            album['folder'] = self.REPLACE_RE.sub('_', '%s-%s' %
                                         (username, album['name']))
            album['ownername'] = username
            if self.extras:
                albumhelpers.get_album_comments(self.query_wrapper,
                                                album, self.friends)
                albumhelpers.add_photo_paths(album)
            self.save_album(album)

    def save_album(self, album):
        self.exit_if_terminated()

        # Create album directory if it doesn't exist
        album_path = os.path.join(self.photos_path, album['folder'])
        if not os.path.isdir(album_path):
            os.mkdir(album_path)

        # Save each photo
        for photo in album['photos'].items():
            # photo is a tuple, [0] pid, [1] dictionary
            self.save_photo(album_path, *photo)

    def save_photo(self, album_path, pid, photo):
        self.exit_if_terminated()

        # Get the file...
        filename = os.path.join(album_path, '%s.jpg' % pid)

        # If file already exists don't download
        if os.path.isfile(filename):
            self.photo_saved(None) # but update the counter
            return

        # Add download task to worker queue
        self.po.apply_async(albumhelpers.download_pic, (photo, filename,),
                            callback = self.photo_saved)

    # Reset modification times
    def fix_album(self, album):
        album_path = os.path.join(self.photos_path, album['folder'])
        os.utime(album_path, (int(album['modified']),) * 2)

    # save_photo task is **successfully** completed
    def photo_saved(self, r):
        self.index += 1
        self.update(self.index, self.total)

    # save_albums_dict is **successfully** completed
    def json_saved(self, r):
        logging.info('JSON files saved successfully')

    # the main show
    def run(self):
        try:
            logging.info('Getting album info')
            self.get_albums()

            logging.info('Getting picture info')
            self.get_pictures()

            self.total = sum(len(album['photos'])
                             for album in self.albums.values())
            # we want it to show that it's not done while we're waiting
            # for albums timestamps fixed and JSON written to disk
            self.total += 1

            if not os.path.isdir(self.photos_path):
                os.makedirs(self.photos_path) # recursive makedir

            # Concurrent download functionality
            self.po = multiprocessing.Pool(processes=5)

            logging.info('Processing and saving albums')
            self.save_albums()

            if self.extras:
                logging.info('Saving JSON dictionaries')
                self.po.apply_async(albumhelpers.save_albums_dict,
                                   (self.albums,
                                    self.friends,
                                    self.photos_path),
                                    callback = self.json_saved)

            # Stop accepting more work
            self.po.close()

            logging.info('Wait for all workers to finish')
            while multiprocessing.active_children():
                self.exit_if_terminated()
                time.sleep(1)
            self.po.join()

            logging.info('Fixing Album Timestamps')
            for album in self.albums.values():
                self.fix_album(album)

        except Exception, e:
            logging.exception('problem in download thread')
            self.exit_if_terminated()
            self.force_exit() # kill GUI
            sys.exit(1) # kill thread
        self.update(self.total,self.total,done=True)