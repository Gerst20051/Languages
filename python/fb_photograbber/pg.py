#!/usr/bin/python
import sys
import os
if hasattr(sys, "winver") and hasattr(sys, "frozen"):
    os.environ["TCL_LIBRARY"] = os.path.join(os.path.dirname(sys.executable),
                                             "tcl",
                                             "tcl8.5")
from Tkinter import *
from tkMessageBox import *
import tkDirectoryChooser
import tkMessageBox
import downloader
import facebook
import traceback
import logging
import multiprocessing

class Application(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.master.title("PhotoGrabber")
        self.master.resizable(width=FALSE, height=FALSE)
        self.master.protocol("WM_DELETE_WINDOW", self.quit_wrapper) #press quit
        self.pack(fill=BOTH, expand=1)
        # fix icon on windows
        if sys.platform == 'win32':
            self.master.iconbitmap(default='dep/pg.ico')
        # downloading thread
        self.dl = None
        self.createWidgets()

    def createWidgets(self):
        # menubar
        mb = Menu(self.master)
        self.master.configure(menu=mb)

        filemenu=Menu(mb,tearoff=0)
        mb.add_cascade(label="File", menu=filemenu)
        filemenu.add_command(label="About", command=self.aboutmsg)
        filemenu.add_command(label="Quit", command=self.quit_wrapper)

        # login button
        imglogin = PhotoImage(file="dep/login.ppm")
        self.bLogin = Button(self, image=imglogin, command=self.fblogin)
        self.bLogin.image = imglogin
        self.bLogin.pack()

        # login textbox
        self.tokenL = Label(self, text="Paste your login token here:")
        self.tokenE = Entry(self)

        # logged in button
        imgcreep = PhotoImage(file="dep/creepon.ppm")
        self.bCreep = Button(self, image=imgcreep, command=self.creep)
        self.bCreep.image = imgcreep

        # list of friends
        self.pFrame = Frame(self)
        self.sb = Scrollbar(self.pFrame, orient=VERTICAL)
        self.lbPeople = Listbox(self.pFrame, yscrollcommand=self.sb.set,
                                selectmode=SINGLE)
        self.sb.config(command=self.lbPeople.yview)
        # check boxes
        self.default_cb = Checkbutton(self.pFrame,
                                      text="All tagged photos of the user")
        self.default_cb.select()
        self.default_cb["state"]=DISABLED
        self.default_cb.pack(fill=X)

        self.full_albums = BooleanVar()
        self.full_cb = Checkbutton(self.pFrame,
                             text="Entire album if it contains a tagged photo",
                             var=self.full_albums)

        self.user_albums = BooleanVar()
        self.user_cb = Checkbutton(self.pFrame,
                                   text="Albums uploaded by the user",
                                   var=self.user_albums)

        self.extras = BooleanVar()
        self.extras_cb = Checkbutton(self.pFrame,
                                     text="Comments and tagging information",
                                     var=self.extras)

        self.full_cb.pack(fill=X)
        self.user_cb.pack(fill=X)
        self.extras_cb.pack(fill=X)
        self.sb.pack(side=RIGHT, fill=Y)
        self.lbPeople.pack(side=RIGHT, fill=BOTH, expand=1)

        # download button
        imgdownload = PhotoImage(file="dep/download.ppm")
        self.bDownload = Button(self, image=imgdownload, command=self.download)
        self.bDownload.image = imgdownload

        # display download status
        self.lDownload = Label(self)

        # quit button
        imgquit = PhotoImage(file="dep/quit.ppm")
        self.bQuit = Button(self, image=imgquit, command=self.do_quit)
        self.bQuit.image = imgquit


    # display about information
    def aboutmsg(self):
        showinfo("About PhotoGrabber", "Developed by Tommy Murphy:\n"
            + "eat.ourbunny.com\n\n"
            + "Contributions from Bryce Boe:\n"
            + "bryceboe.com\n\n"
            + "Icons:\neveraldo.com/crystal")

    # login button event
    def fblogin(self):
        try:
            logging.info("Redirecting user to browser to login.")
            facebook.getToken()
        except Exception, e:
            self.error(e)

        # hide login button
        self.bLogin["state"]=DISABLED
        self.bLogin.pack_forget()
        # select a user button
        self.tokenL.pack(fill=X)
        self.tokenE.pack(fill=X)
        self.bCreep.pack(fill=X)

    # load the list of friends
    def creep(self):
        try:
            if self.tokenE.get() == "":
                facebook.getToken() # in case the first time didn't work
                return
            self.graph = facebook.GraphAPI(self.tokenE.get())
            try:
                self.profile = self.graph.get_object('me')
                friends = self.graph.get_object('me/friends')['data']
            except Exception, e:
                m = ''.join(['There was a problem connecting to facebook,',
                            ' please try again:\n\n %s'])
                showinfo("Error", m % e)
                return

            self.people = sorted(friends, key=lambda k:k['name'].lower())

            for person in self.people :
                name = person['name']
                self.lbPeople.insert(END, name)

            me = dict(id=self.profile['id'],name="Myself")
            self.lbPeople.insert(0, "Myself")
            self.people.insert(0,me)

            # show the list of people
            self.pFrame.pack(fill=X)

        except Exception, e:
            self.error(e)

        self.tokenE["state"]=DISABLED
        self.bCreep["state"]=DISABLED
        self.bDownload.pack()


    # download button event
    def download(self):
        # get listbox selection before directory prompt
        item = self.lbPeople.curselection()
        # ask for a directory
        self.directory = tkDirectoryChooser.askdirectory()
        logging.info('directory: %s' % self.directory)

        # show the fb login button
        if self.directory != "":
            self.lbPeople["state"]=DISABLED
            self.full_cb["state"]=DISABLED
            self.user_cb["state"]=DISABLED
            self.extras_cb["state"]=DISABLED

            # check listbox selection
            if len(item) == 1:
                uid = self.people[int(item[0])]['id']
            else:
                uid = self.profile['id']

            # make dictonary of friends
            friends = dict((x['id'], x['name']) for x in self.people)

            # download
            logging.info('starting FBDownloader thread')
            self.dl = downloader.FBDownloader(self.directory, uid, friends,
                                              self.full_albums.get(),
                                              self.user_albums.get(),
                                              self.extras.get(),
                                              self.graph,
                                              self.update_status,
                                              self.remote_exit)
            self.dl.start()

            self.bDownload["state"] = DISABLED
            self.lDownload["text"] = ''.join(["Collecting photo list... ",
                                              "(this may take a while)"])
            self.lDownload.pack()

    # update download status function - callback from thread
    def update_status(self, index, total, done=False):
        self.lDownload["text"] = '%s of %s' % (index, total)
        self.lDownload.pack()

        if done:
            logging.info('all done!')
            self.bQuit.pack()

    # oops an error happened! - callback from thread
    def error(self, e):
        logging.exception('There was a problem')
        showinfo("ERROR!", "Non-recoverable error, try again!\n\n %s" % e)
        self.quit()

    # handle request to exit - callback from thread
    def remote_exit(self):
        logging.info('thread requests exit... destorying widgets')
        self.quit() # destroy widgets

    # quit **button** event
    def do_quit(self):
        self.bQuit["state"] = DISABLED
        if self.dl.isAlive(): self.dl.join()
        self.dl = None
        self.quit()

    # window manager quit - callback from UI
    def quit_wrapper(self):
        if self.dl:
            if tkMessageBox.askyesno(title="Quit During Download?",
                    message="Are you sure you want to quit during a download?") == 0:
                return
            self.dl._thread_terminated = True
            logging.debug('Waiting for download thread to terminate')
            while self.dl.isAlive():
                logging.debug('.')
                self.dl.join(1)
            self.dl = None
            logging.debug('Thread terminated')
        self.quit()

LEVELS = {'debug': logging.DEBUG,
          'info': logging.INFO,
          'warning': logging.WARNING,
          'error': logging.ERROR,
          'critical': logging.CRITICAL}

def main(level=logging.ERROR):
    logging.basicConfig(level=level)
    app = Application()
    try:
        app.mainloop()
    except KeyboardInterrupt:
        if app.dl: app.dl._thread_terminated = True
    if app.dl: app.dl.join()

if __name__ == '__main__':
    # add freeze support
    multiprocessing.freeze_support()
    level = logging.ERROR
    if len(sys.argv) > 1:
        level_name = sys.argv[1]
        level = LEVELS.get(level_name, logging.NOTSET)
    main(level)