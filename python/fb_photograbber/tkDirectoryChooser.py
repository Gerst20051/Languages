#
# tkDirectoryChooser.py
# $Id: tkinter-directory-chooser.txt 3403 2008-03-25 22:53:59Z fredrik $
#
# tk common directory dialogue
#
# this module provides interfaces to the native directory dialogue
# available in Tk 8.3 and newer.
#
# written by Fredrik Lundh, November 2000.
#

#
# options (all have default values):
#
# - initialdir: initial directory.  preserved by dialog instance.
#
# - mustexist: if true, user must pick an existing directory
#
# - parent: which window to place the dialog on top of
#
# - title: dialog title
#

from tkCommonDialog import Dialog

class Chooser(Dialog):

    command = "tk_chooseDirectory"

    def _fixresult(self, widget, result):
        if result:
            # keep directory until next time
            self.options["initialdir"] = result
        self.directory = result # compatibility
        return result

#
# convenience stuff

def askdirectory(**options):
    "Ask for a directory name"

    return unicode(apply(Chooser, (), options).show())