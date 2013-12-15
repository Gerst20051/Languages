#! /usr/bin/env python
# -*- mode: python; c-basic-offset: 2; indent-tabs-mode: nil; -*-
# vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
#
# Copyright (C) 2009 Sun Microsystems
# Copyright (C) 2011 Patrick Crews
#
# Authors:
#
#  Jay Pipes <joinfu@sun.com>
#  Monty Taylor <mordred@sun.com>
#  Patrick Crews 
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
#
# This code is modified from the logging module used in the 
# drizzle-automation project - https://launchpad.net/drizzle-automation


""" Simple replacement for python logging module that doesn't suck """

import time, sys


class loggingManager():
    """ Class to deal with logging
        We make a class just because we forsee ourselves being
        multi-threaded and it will be nice to have a single
        point of control for managing i/o.

        Also, this is the cleanest way I can think of to deal
        with test-reporting (again, multi-threaded and such

    """

    def __init__(self, variables):

        self.log_file = sys.stdout
        self.report_fmt = '{0:<55} {1} {2:>12}'
        self.report_started = 0  
        self.thick_line = '='*80
        self.thin_line = '-'*80     

    def _write_message(self,level, msg):
      self.log_file.write("%s %s: %s\n" % (time.strftime("%d %b %Y %H:%M:%S"), level, str(msg)))
      self.log_file.flush()

    def setOutput(self,file_name):
      if file_name == 'stdout':
        self.log_file= sys.stdout
      else:
        self.log_file= open(variables['log_file'],'w+')

    def info(self,msg):
      self._write_message("INFO", msg)

    def warning(self,msg):
      self._write_message("WARNING", msg)

    def error(self,msg):
      self._write_message("ERROR", msg)

    def verbose(self,msg):
      self._write_message("VERBOSE", msg)

    def debug(self,msg):
      self._write_message("DEBUG", msg)
 
    def debug_class(self,codeClass):
        self._write_message("DEBUG**",codeClass)
        skip_keys = ['skip_keys', 'debug', 'verbose']
        for key, item in sorted(vars(codeClass).items()):
            if key not in codeClass.skip_keys and key not in skip_keys:
                self._write_message("DEBUG**",("%s: %s" %(key, item)))



    def test_report( self, test_name, test_status
                   , execution_time, additional_output=None):
        """ We use this method to deal with writing out the test report

        """
        if not self.report_started:
            self.report_started = 1
            self.write_report_header()
        test_status = "[ %s ]" %(test_status)
        msg = self.report_fmt.format( test_name, test_status
                                    , execution_time)
        self._write_message("", msg)
        if additional_output and test_status != '[ pass ]':
            additional_output=additional_output.split('\n')
            for line in additional_output:
                line = line.strip()
                self._write_message("",line)

    def write_report_header(self):
        self.write_thick_line()
        self.test_report("TEST NAME", "RESULT", "TIME (ms)")
        self.write_thick_line()

    def write_thin_line(self):
        self._write_message("",self.thin_line)

    def write_thick_line(self):
        self._write_message("",self.thick_line)




