from distutils.core import setup
import py2exe, sys, os

sys.argv.append('py2exe')

setup(
        data_files = [("dep",["dep/creepon.ppm",
                              "dep/download.ppm",
                              "dep/login.ppm",
                              "dep/quit.ppm",
                              "dep/pg.ico",
                              "dep/viewer.html",])],
        options = {'py2exe': {'optimize':2}},
        windows = [{'script': 'pg.py'}],
        zipfile = None,
)