Program:EXPLORE
ClrHome
Lb1 A
getKey->X
If X=24: Goto 2
If X=25: Goto 1
If X=26: Goto 3
If X=34: Goto 4
If X=105: Goto 5
Goto A
Lb1 1
ClrHome
Output (4,5, "FOREWARD")
Send ({122,100})
Get (R)
Goto A
Lbl 2
ClrHome
Output (4,7, "LEFT")
Send ({102,42})
Get (R)
Goto A
Lbl 3
ClrHome
Output (4,6, "RIGHT")
Send ({120,42})
2
Get (R)
Goto A
Lbl 4
ClrHome
Output (4,5, "BACKWARD")
Send ({100,100})
Get (R)
Goto A
Lbl 5
Stop