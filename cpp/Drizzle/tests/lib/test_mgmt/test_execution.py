#! /usr/bin/env python
# -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
# vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
#
# Copyright (C) 2010 Patrick Crews
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

""" test_execution:
    code related to the execution of est cases 
    
    We are provided access to a testManager with 
    mode-specific testCases.  We contact the executionManager
    to produce the system and server configurations we need
    to execute a test.

"""

# imports
import os
import sys
import subprocess

class testExecutor():
    """ class for handling the execution of testCase
        objects.  Mode-specific executors inherit
        from me.
 
    """

    def __init__(self, execution_manager, name, verbose, debug):
        self.skip_keys = [ 'execution_manager'
                         , 'system_manager'
                         , 'test_manager'
                         , 'server_manager']
        self.debug = debug
        self.verbose = verbose
        self.initial_run = 1
        self.status = 0 # not running
        self.execution_manager = execution_manager
        self.system_manager = self.execution_manager.system_manager
        self.testcase_repeat_count = self.execution_manager.testcase_repeat_count
        self.cmd_prefix = self.system_manager.cmd_prefix
        self.logging = self.system_manager.logging
        self.test_manager = self.execution_manager.test_manager
        self.server_manager = self.execution_manager.server_manager
        self.time_manager = self.system_manager.time_manager
        self.name = name
        self.working_environment = {} # we pass env dict to define what we need
        self.dirset = { self.name : { 'log': None } }
        self.workdir = self.system_manager.create_dirset( self.system_manager.workdir
                                                        , self.dirset)
        self.logdir = os.path.join(self.workdir,'log')
        self.master_server = self.server_manager.allocate_server( self.name
                                                                , []
                                                                , self.workdir
                                                                )
        self.record_flag=self.execution_manager.record_flag
        self.current_servers = [self.master_server]
        self.current_testcase = None    
        self.current_test_status = None
        self.current_test_retcode = None
        self.current_test_output = None
        self.current_test_exec_time = 0 
         
        if self.debug:
            self.logging.debug_class(self)

    def execute(self, start_and_exit):
        """ Execute a test case.  The details are *very* mode specific """
        self.status = 1 # we are running
        keep_running = 1
        if self.verbose:
            self.logging.verbose("Executor: %s beginning test execution..." %(self.name))
        while self.test_manager.has_tests() and keep_running == 1:
            self.get_testCase()
            for i in range(self.testcase_repeat_count):
                self.handle_system_reqs()
                self.handle_server_reqs()
                self.handle_utility_reqs()
                self.handle_start_and_exit(start_and_exit)
                if self.current_test_status != 'fail':
                    self.execute_testCase()
                self.record_test_result()
                if self.current_test_status == 'fail' and not self.execution_manager.force:
                    self.logging.error("Failed test.  Use --force to execute beyond the first test failure")
                    keep_running = 0
                self.current_test_status = None # reset ourselves
        self.status = 0

    def get_testCase(self):
        """ Ask our execution_manager for a testCase to work on """
        
        #self.test_manager.mutex.acquire()
        self.current_testcase = self.test_manager.get_testCase(self.name)
        #self.test_manager.mutex.release()
        

    def handle_server_reqs(self):
        """ Get the servers required to execute the testCase 
            and ensure that we have servers and they were started
            as expected.  We take necessary steps if not
            We also handle --start-and-exit here
 
        """

        server_requirements = self.current_testcase.server_requirements
        (self.current_servers,bad_start) = self.server_manager.request_servers( self.name
                                                              , self.workdir
                                                              , server_requirements
                                                              , self.working_environment)
        if self.current_servers == 0 or bad_start:
            # error allocating servers, test is a failure
            self.logging.warning("Problem starting server(s) for test...failing test case")
            self.current_test_status = 'fail'
            self.set_server_status(self.current_test_status)
            output = ''           
        if self.initial_run:
            self.initial_run = 0
            self.current_servers[0].report()
        self.master_server = self.current_servers[0]
        if len(self.current_servers) > 1:
            # We have a validation server or something we need to communicate with
            # We export some env vars with EXECUTOR_SERVER and expect the randge
            # code to know enough to look for this marker
            extra_reqs = {}
            for server in self.current_servers:
                variable_name = "%s_%s" %(self.name.upper(), server.name.upper())
                variable_value = str(server.master_port)
                extra_reqs[variable_name] = variable_value
            self.working_environment.update(extra_reqs)
        return 

    def handle_start_and_exit(self, start_and_exit):
        """ Do what needs to be done if we have the
            --start-and-exit flag

        """
        if start_and_exit:
                # We blow away any port_management files for our ports
                # Technically this won't let us 'lock' any ports that 
                # we aren't explicitly using (visible to netstat scan)
                # However one could argue that if we aren't using it, 
                # We shouldn't hog it ; )
                # We might need to do this better later
                for server in self.current_servers:
                    if server != self.master_server:
                        server.report()
                    server.cleanup() # this only removes any port files
                self.logging.info("User specified --start-and-exit.  dbqp.py exiting and leaving servers running...") 
                sys.exit(0)

    def handle_utility_reqs(self):
        """ Call any utilities we want to use before starting a test
            At present this is envisioned for use with datagen
            but there may be other things we wish to use
            At that point, we may need to explore other ways of
            defining our testing environment, such as with
            nice config files / modules

        """

        # We call gendata against the server(s) with the
        # specified file
        if self.execution_manager.gendata_file:
            dsn = "--dsn=dbi:drizzle:host=localhost:port=%d:user=root:password="":database=test" %(self.master_server.master_port)
            gendata_cmd = "./gendata.pl %s --spec=%s" %( dsn 
                                                       , self.execution_manager.gendata_file
                                                       )
            #self.system_manager.execute_cmd(gendata_cmd)
            gendata_subproc = subprocess.Popen( gendata_cmd
                                              , shell=True
                                              , cwd=self.system_manager.randgen_path
                                              , stdout = None
                                              , stderr = None
                                              )
            gendata_subproc.wait()
            gendata_retcode = gendata_subproc.returncode
            if gendata_retcode:
                self.logging.error("gendata command: %s failed with retcode: %d" %(gendata_cmd
                                                                             , gendata_retcode))

    def execute_testCase(self):
        """ Do whatever evil voodoo we must do to execute a testCase """
        if self.verbose:
            self.logging.verbose("Executor: %s executing test: %s" %(self.name, self.current_testcase.fullname))

    def record_test_result(self):
        """ We get the test_manager to record the result """

        self.test_manager.record_test_result( self.current_testcase
                                                , self.current_test_status
                                                , self.current_test_output
                                                , self.current_test_exec_time )

            
    def set_server_status(self, test_status):
        """ We update our servers to indicate if a test passed or failed """
        for server in self.current_servers:
            if test_status == 'fail':
                server.failed_test = 1

   
    def handle_system_reqs(self):
        """ We check our test case and see what we need to do
            system-wise to get ready.  This is likely to be 
            mode-dependent and this is just a placeholder
            method

        """
        
        return

   
