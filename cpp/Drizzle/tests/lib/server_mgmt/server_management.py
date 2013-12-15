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

"""server_management.py
   code for dealing with apportioning servers
   to suit the needs of the tests and executors

"""
# imports
import thread
import time
import os
import subprocess

class serverManager:
    """ code that handles the server objects
        We use this to track, do mass actions, etc
        Single point of contact for this business

    """

    def __init__(self, system_manager, variables):
        self.skip_keys = [ 'ld_lib_paths'
                         , 'system_manager'
                         , 'logging'
                         , 'gdb'
                         , 'code_tree'
                         ]
        self.debug = variables['debug']
        self.verbose = variables['verbose']
        self.initial_run = 1
        self.server_base_name = 'server'
        self.no_secure_file_priv = variables['nosecurefilepriv']
        self.system_manager = system_manager
        self.logging = system_manager.logging
        self.gdb  = self.system_manager.gdb
        self.code_tree = system_manager.code_tree
        self.default_storage_engine = variables['defaultengine']
        self.user_server_opts = variables['drizzledoptions']
        self.servers = {}
        # We track this
        self.ld_lib_paths = system_manager.ld_lib_paths
        self.mutex = thread.allocate_lock()
        self.timer_increment = .5

        self.logging.info("Using default-storage-engine: %s" %(self.default_storage_engine))

        if self.debug:
            self.logging.debug_class(self)

    def request_servers( self, requester, workdir, server_requirements
                       , working_environ, expect_fail = 0):
        """ We produce the server objects / start the server processes
            as requested.  We report errors and whatnot if we can't
            That is, unless we expect the server to not start, then
            we just return a value / message.

            server_requirements is a list of lists.  Each list
            is a set of server options - we create one server
            for each set of options requested
    
        """

        # Make sure our server is in a decent state, if the last test
        # failed, then we reset the server
        self.check_server_status(requester)
        
        # Make sure we have the proper number of servers for this requester
        self.process_server_count( requester, len(server_requirements)
                                 , workdir, server_requirements)

        # Make sure we are running with the correct options 
        self.evaluate_existing_servers(requester, server_requirements)

        # Fire our servers up
        bad_start = self.start_servers( requester, working_environ
                                      , expect_fail)
 
        # Return them to the requester
        return (self.get_server_list(requester), bad_start)        


    
    def allocate_server(self, requester, server_options, workdir):
        """ Intialize an appropriate server object.
            Start up occurs elsewhere

        """

        # Get a name for our server
        server_name = self.get_server_name(requester)

        # initialize our new server_object
        if self.code_tree.type == 'Drizzle':
          from lib.server_mgmt.drizzled import drizzleServer as server_type
        new_server = server_type( server_name
                                , self
                                , self.default_storage_engine
                                , server_options
                                , requester
                                , workdir )
        return new_server

    def start_servers(self, requester, working_environ, expect_fail):
        """ Start all servers for the requester """
        bad_start = 0
        for server in self.get_server_list(requester):
            if server.status == 0:
                bad_start = bad_start + self.start_server( server
                                                         , requester
                                                         , working_environ
                                                         , expect_fail
                                                         )
        return bad_start

    def start_server(self, server, requester, working_environ, expect_fail):
        """ Start an individual server and return
            an error code if it did not start in a timely manner
 
            Start the server, using the options in option_list
            as well as self.standard_options
            
            if expect_fail = 1, we know the server shouldn't 
            start up

        """

        if self.verbose:
            self.logging.verbose("Starting server: %s.%s" %(server.owner, server.name))
        start_cmd = server.get_start_cmd()
        if self.debug:
            self.logging.debug("Starting server with:")
            self.logging.debug("%s" %(start_cmd))
        # we signal we tried to start as an attempt
        # to catch the case where a server is just 
        # starting up and the user ctrl-c's
        # we don't know the server is running (still starting up)
        # so we give it a few minutes
        self.tried_start = 1
        error_log = open(server.error_log,'w')
        if start_cmd: # It will be none if --manual-gdb used
            if not self.gdb:
                server_subproc = subprocess.Popen( start_cmd
                                                 , shell=True
                                                 , env=working_environ
                                                 , stdout=error_log
                                                 , stderr=error_log
                                                 )
                server_subproc.wait()
                server_retcode = server_subproc.returncode
            else: 
                # This is a bit hackish - need to see if there is a cleaner
                # way of handling this
                # It is annoying that we have to say stdout + stderr = None
                # We might need to further manipulate things so that we 
                # have a log
                server_subproc = subprocess.Popen( start_cmd
                                                 , shell=True
                                                 , env = working_environ
                                                 , stdin=None
                                                 , stdout=None
                                                 , stderr=None
                                                 , close_fds=True
                                                 )
        
                server_retcode = 0
        else:
            # manual-gdb issue
            server_retcode = 0
        timer = float(0)
        timeout = float(server.server_start_timeout)

        #if server_retcode: # We know we have an error, no need to wait
        #    timer = timeout

        

        while not server.is_started() and timer != timeout:
            time.sleep(self.timer_increment)
            # If manual-gdb, this == None and we want to give the 
            # user all the time they need
            if start_cmd:
                timer= timer + self.timer_increment
            
        if timer == timeout and not self.ping_server(server, quiet=True):
            self.logging.error(( "Server failed to start within %d seconds.  This could be a problem with the test machine or the server itself" %(timeout)))
            server_retcode = 1
     
        if server_retcode == 0:
            server.status = 1 # we are running

        if server_retcode != 0 and not expect_fail and self.debug:
            self.logging.debug("Server startup command: %s failed with error code %d" %( start_cmd
                                                                                  , server_retcode))
        elif server_retcode == 0 and expect_fail:
        # catch a startup that should have failed and report
            self.logging.error("Server startup command :%s expected to fail, but succeeded" %(start_cmd))

        server.tried_start = 0 
        return server_retcode ^ expect_fail

    def ping_server(self, server, quiet=False):
        """ Ping / check if the server is alive 
            Return True if server is up and running, False otherwise
        """
 
        ping_cmd = server.get_ping_cmd()
        if not quiet:
            self.logging.info("Pinging Drizzled server on port %d" % server.master_port)
        (retcode, output)= self.system_manager.execute_cmd(ping_cmd, must_pass = 0)
        return retcode == 0
             

    def stop_server(self, server):
        """ Stop an individual server if it is running """
        if server.tried_start:
            # we expect that we issued the command to start
            # the server but it isn't up and running
            # we kill a bit of time waiting for it
            attempts_remain = 10
            while not self.ping_server(server, quiet=True) and attempts_remain:
                time.sleep(1)
                attempts_remain = attempts_remain - 1
        # Now we try to shut the server down
        if self.ping_server(server, quiet=True):
            if self.verbose:
                self.logging.verbose("Stopping server %s.%s" %(server.owner, server.name))
            stop_cmd = server.get_stop_cmd()
            retcode, output = self.system_manager.execute_cmd(stop_cmd)
            if retcode:
                self.logging.error("Problem shutting down server:")
                self.logging.error("%s : %s" %(retcode, output))
            else:
                server.status = 0 # indicate we are shutdown
        else:
            # make sure the server is indicated as stopped
            server.status = 0

    def stop_servers(self, requester):
        """ Stop all servers running for the requester """
        for server in self.get_server_list(requester):
            self.stop_server(server)

    def stop_server_list(self, server_list):
        """ Stop the servers in an arbitrary list of them """
        for server in server_list:
            self.stop_server(server)

    def stop_all_servers(self):
        """ Stop all running servers """

        self.logging.info("Stopping all running servers...")
        for server_list in self.servers.values():
            for server in server_list:
                self.stop_server(server)

    def cleanup_all_servers(self):
        """Mainly for freeing server ports for now """
        for server_list in self.servers.values():
            for server in server_list:
                server.cleanup()

    def cleanup(self):
        """Stop all servers and free their ports and whatnot """
        self.stop_all_servers()
        self.cleanup_all_servers()

    def get_server_name(self, requester):
        """ We name our servers requester.server_basename.count
            where count is on a per-requester basis
            We see how many servers this requester has and name things 
            appropriately

        """
        self.has_servers(requester) # if requester isn't there, we create a blank entry
        server_count = self.server_count(requester)
        return "%s%d" %(self.server_base_name, server_count)

    def has_servers(self, requester):
        """ Check if the given requester has any servers """
        if requester not in self.servers: # new requester
           self.log_requester(requester) 
        return self.server_count(requester)

    def log_requester(self, requester):
        """ We create a log entry for the new requester """

        self.servers[requester] = []

    def log_server(self, new_server, requester):
        self.servers[requester].append(new_server)

    def evaluate_existing_servers( self, requester, server_requirements):
        """ See if the requester has any servers and if they
            are suitable for the current test

            We should have the proper number of servers at this point

        """
        current_servers = self.servers[requester]

        for index,server in enumerate(current_servers):
            server_options = server_requirements[index]
            # We add in any user-supplied options here
            server_options = server_options + self.user_server_opts
            if self.compare_options( server.server_options
                                   , server_options):
                return 1
            else:
                # We need to reset what is running and change the server
                # options
                server_options = self.filter_server_options(server_options)
                self.reset_server(server)
                self.update_server_options(server, server_options)


       

    def filter_server_options(self, server_options):
        """ Remove a list of options we don't want passed to the server
            these are test-case specific options.
 
            NOTE: It is a bad hack to allow test-runner commands
            to mix with server options willy-nilly in master-opt files
            as we do.  We need to kill this at some point : (

        """
        remove_options = [ '--restart'
                         , '--skip-stack-trace'
                         , '--skip-core-file'
                         , '--'
                         ]
        for remove_option in remove_options:
            if remove_option in server_options:
                server_options.remove(remove_option)
        return server_options
            
    
    def compare_options(self, optlist1, optlist2):
        """ Compare two sets of server options and see if they match """
        return sorted(optlist1) == sorted(optlist2)

    def reset_server(self, server):
        self.stop_server(server)
        server.restore_snapshot()
        server.reset()

    def reset_servers(self, requester):
        for server in self.servers[requester]:
            self.reset_server(server)


    def process_server_count(self, requester, desired_count, workdir, server_reqs):
        """ We see how many servers we have.  We shrink / grow
            the requesters set of servers as needed.

            If we shrink, we shutdown / reset the discarded servers
            (naturally)
 
        """
        if desired_count < 0:  desired_count = 1

        current_count = self.has_servers(requester)
        if desired_count > current_count:
            for i in range(desired_count - current_count):
                # We pass an empty options list when allocating
                # We'll update the options to what is needed elsewhere
                self.allocate_server(requester,[] , workdir)
        elif desired_count < current_count:
            good_servers = self.get_server_list(requester)[:desired_count]
            retired_servers = self.get_server_list(requester)[desired_count - current_count:]
            self.stop_server_list(retired_servers)
            self.set_server_list(requester, good_servers)
            
         

    def server_count(self, requester):
        """ Return how many servers the the requester has """
        return len(self.servers[requester])

    def get_server_list(self, requester):
        """ Return the list of servers assigned to the requester """
        self.has_servers(requester) # initialize, hacky : (
        return self.servers[requester]
 
    def set_server_list(self, requester, server_list):
        """ Set the requesters list of servers to server_list """

        self.servers[requester] = server_list

    def add_server(self, requester, new_server):
       """ Add new_server to the requester's set of servers """
       self.servers[requester].append(new_server)

    def update_server_options(self, server, server_options):
        """ Change the option_list a server has to use on startup """
        if self.debug:
            self.logging.debug("Updating server: %s options" %(server.name))
            self.logging.debug("FROM: %s" %(server.server_options))
            self.logging.debug("TO: %s" %(server_options))
        server.set_server_options(server_options)

    def get_server_count(self):
        """ Find out how many servers we have out """
        server_count = 0
        for server_list in self.servers.values():
            for server in server_list:
                server_count = server_count + 1
        return server_count

    def check_server_status(self, requester):
        """ Make sure our servers are good,
            reset the otherwise.

        """
        for server in self.get_server_list(requester):
            if server.failed_test:
                self.reset_server(server)





        








