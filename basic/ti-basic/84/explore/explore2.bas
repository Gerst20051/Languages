Program: EXPLORE2
ClrHome
Lb1 A
getKey->X
If X=73: Goto 1
If X=93: Goto 2
If X=82: Goto 3
If X=84: Goto 4
If X=74: Goto 5
If X=94: Goto 6
If X=72: Goto 7
If X=92: Goto 8
If X=83: Goto 9
If X=105:Goto 1
0
Goto A
Lb1 1
ClrHome
Output (4,5, "FOREWARD")
Send ({122,99})
Get (R)
Goto A
Lbl 3
ClrHome
Output (4,7, "LEFT")
5
Send ({102,43})
Get (R)
Goto A
Lbl 4
ClrHome
Output (4,6, "RIGHT")
Send ({120,42})
Get (R)
Goto A
Lbl 2
ClrHome
Output (4,5, "BACKWARD")
Send ({100,99})
Get (R)
Goto A
Lbl 5
ClrHome
Output (4,5, "FORRIGHT")
Send ({120,22})
Get (R)
Goto A
Lbl 6
ClrHome
Output (4,4, "BACKRIGHT")
Send ({120,67})
Get (R)
Goto A
Lbl 7
ClrHome
Output (4,5, "FORLEFT")
Send ({102,22})
Get (R)
Goto A
Lbl 8
ClrHome
Output (4,5, "BACKLEFT")
Send ({102,67})
Get (R)
Goto A
Lbl 9
6
ClrHome
Output (4,6, "DEFENCE")
Send ({120,99})
Get (R)
Send ({102,99})
Get (R)
Goto A
Lbl 10
Stop