C:\> Set foo="this string is enclosed in quotes"
 
C:\> Set foo=%foo:"="""%
 
C:\> Echo "test 1 %foo%"
"test 1 """this string is enclosed in quotes""""
 
C:\> Eventcreate /T Warning /ID 1 /L System /SO "Source" /D "Example: %foo%"
SUCCESS: A 'Warning' type event is created in the 'Source' log/source.