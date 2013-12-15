Name: SilentStart
Version: 0.1
Author: Robert.A.Davies (bobbigmac) admin@robertadavies.co.uk
Link: http://www.robertadavies.co.uk

Description:
SilentStart is a very simple wrapper for the windows CreateProcess api call.

Instructions:
First command-line parameter should be the name or full path of the application you wish to execute.
All remaining parameters will be passed to that executable.
The SilentStart process waits for the silently called process to complete before terminating.
The exit code will be 1 if application fails to locate the executable and make the createprocess call.
The exit code will be returned from the called application if the call is made successfully.

Known issues:
1) Validation of command-line could be improved to check path format and existence of requested executable.
2) Attempts to write errors to stdout are non-functional (uses MessageBox as fallback).

Notes:
My C++ skills are limited at best, please ensure you check the source for problems or potential overflows before using in any production environment.

Licence: CC-BY-SA
Creative Commons Attribution Sharealike
http://creativecommons.org/licenses/by-sa/2.0/
No guarantees. Use entirely at your own risk.
It would be nice if you let me know when/where you are using it and of any issues you encounter.