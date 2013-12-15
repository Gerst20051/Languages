#! /usr/bin/env python
# -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
# vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
#
# Copyright (C) 2010 Patrick Crews
#
## This program is free software; you can redistribute it and/or modify
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

""" randgen_test_management:
    code related to the gathering / analysis / management of 
    the test cases
    ie - collecting the list of tests in each suite, then
    gathering additional, relevant information for randgen
    mode. (stocastic model-based testing)

"""

# imports
import os
import re
import sys
from ConfigParser import RawConfigParser

import lib.test_mgmt.test_management as test_management


    
class testCase:
    """Holds info on a single randgen test
 
    """
    def __init__( self, system_manager, name=None
                , fullname = None, server_requirements=[[]]
                , comment=None, test_command=None, debug=False ):
        self.system_manager = system_manager
        self.logging = self.system_manager.logging
        self.skip_keys = ['system_manager','logging']
        self.name = name
        self.fullname = fullname
        self.suitename = 'randgen_tests'
        self.master_sh = None
        self.comment = comment
        self.server_requirements = server_requirements
        self.test_command = test_command
        
        if debug:
            self.system_manager.logging.debug_class(self)

    def should_run(self):
        if self.skip_flag or self.disable:
            return 0
        else:
            return 1

 
        
        
          
class testManager(test_management.testManager):
    """Deals with scanning test directories, gathering test cases, and 
       collecting per-test information (opt files, etc) for use by the
       test-runner

    """

    def __init__(self, verbose, debug, default_engine, dotest, skiptest
                , reorder, suitelist, suitepaths, system_manager
                , test_cases, mode):
        super(testManager, self).__init__( verbose, debug, default_engine
                                         , dotest, skiptest, reorder
                                         , suitelist, suitepaths
                                         , system_manager, test_cases, mode)
        self.suitepaths = [os.path.join(self.testdir,'randgen_tests')]
        if suitelist is None:
            self.suitelist = ['main']
        else:
            self.suitelist = suitelist

    def process_suite(self,suite_dir):
        """Process a test suite.
           Look for randgen tests, which are nice clean conf files
        
        """

        # We know this based on how we organize randgen test conf files
        suite_name = os.path.basename(suite_dir) 
        if self.verbose:
                self.system_manager.logging.verbose("Processing suite: %s" %(suite_name))
        testlist = [os.path.join(suite_dir,test_file) for test_file in sorted(os.listdir(suite_dir)) if test_file.endswith('.cnf')]

        # Search for specific test names
        if self.desired_tests: # We have specific, named tests we want from the suite(s)
           tests_to_use = []
           for test in self.desired_tests:
               if test.endswith('.cnf'): 
                   pass
               else:
                   test = test+'.cnf'
               test = os.path.join(suite_dir,test)
               if test in testlist:
                   tests_to_use.append(test)
           testlist = tests_to_use
        for test_case in testlist:
            self.add_test(self.process_test_file(suite_name, test_case))


    def process_test_file(self, suite_name, testfile):
        """ We convert the info in a testfile into a testCase object """

        config_reader = RawConfigParser()
        config_reader.read(testfile)
        # test_name = filename - .cnf...simpler
        test_name = os.path.basename(testfile).replace('.cnf','')
        test_comment = config_reader.get('test_info','comment')
        server_requirements = self.process_server_reqs(config_reader.get('test_servers','servers'))
        test_command = config_reader.get('test_command','command')
        return testCase( self.system_manager
                       , name = test_name
                       , fullname = "%s.%s" %(suite_name, test_name)
                       , server_requirements = server_requirements
                       , test_command = test_command
                       , debug = self.debug )

        #sys.exit(0)

    def process_server_reqs(self,data_string):
        """ We read in the list of lists as a string, so we need to 
            handle this / break it down into proper chunks

        """
        server_reqs = []
        # We expect to see a list of lists and throw away the 
        # enclosing brackets
        option_sets = data_string[1:-1].strip().split(',')
        for option_set in option_sets:
            server_reqs.append([option_set[1:-1].strip()])
        return server_reqs
