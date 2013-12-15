C:\> Set foo="this string is enclosed in quotes"
 
C:\> Echo "test 1 %foo%"
"test 1 "this string is enclosed in quotes""
 
C:\> Eventcreate /T Warning /ID 1 /L System /SO "Source" /D "Example: %foo%"
ERROR: Invalid Argument/Option - 'string'.
Type "EVENTCREATE /?" for usage.