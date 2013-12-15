Program:ROBOT
:ClrHome:ClrDraw:AxesOff:FnOff
:-50->Xmin:50->Xmax:1->Xscl:-50->Ymin:50->Ymax:1->Yscl
:Vertical -50:Vertical 50:Horizontal 50:Horizontal -50
:0->A:0->B:0->Y:0->Z
:Pt-On(A,B,3):Pt-On(A,B+3
:While 1
:getKey->Z
:If Z=45:Stop
:If Z:Then
:Pt-Off(A,B,3
:If Y=0:Pt-Off(A,B+3
:If Y=1:Then:Pt-Off(A-1,B+1):Pt-Off(A-2,B+3):End
:If Y=2:Pt-Off(A-3,B
:If Y=3:Then:Pt-Off(A-1,B-1):Pt-Off(A-2,B-3):End
:If Y=4:Pt-Off(A,B-3
:If Y=5:Then:Pt-Off(A+1,B-1):Pt-Off(A+2,B-3):End
:If Y=6:Pt-Off(A+3,B
:If Y=7:Then:Pt-Off(A+1,B+1):Pt-Off(A+2,B+3):End
:If Z=24:Then
:If Yø7:Then:Y+1->Y
:Else 0->Y:End:End
:If Z=26:Then
:If Yø0:Then:Y-1->Y
:Else 7->Y:End:End
:If Z=25:Then
:If Y=0:B+1->B
:If Y=1:Then:A-1->A:B+1->B:End
:If Y=2:A-1->A
:If Y=3:Then:A-1->A:B-1->B:End
:If Y=4:B-1->B
:If Y=5:Then:A+1->A:B-1->B:End
:If Y=6:A+1->A
:If Y=7:Then:A+1->A:B+1->B:End
:End
:If Z=34:Then
:If Y=0:B-1->B
:If Y=1:Then:A+1->A:B-1->B:End
:If Y=2:A+1->A
:If Y=3:Then:A+1->A:B+1->B:End
:If Y=4:B+1->B
:If Y=5:Then:A-1->A:B+1->B:End
:If Y=6:A-1->A
:If Y=7:Then:A-1->A:B-1->B:End
:End
:If A<-46:-46->A
:If A>46:46->A
:If B<-44:-44->B
:If B>44:44->B
:Text(1,2,"(",A,",",B,")    "
:Text(1,50,Y
:Pt-On(A,B,3
:If Y=0:Pt-On(A,B+3
:If Y=1:Then:Pt-On(A-1,B+1):Pt-On(A-2,B+3):End
:If Y=2:Pt-On(A-3,B
:If Y=3:Then:Pt-On(A-1,B-1):Pt-On(A-2,B-3):End
:If Y=4:Pt-On(A,B-3
:If Y=5:Then:Pt-On(A+1,B-1):Pt-On(A+2,B-3):End
:If Y=6:Pt-On(A+3,B
:If Y=7:Then:Pt-On(A+1,B+1):Pt-On(A+2,B+3):End
:Send({l,r})
:End
:End