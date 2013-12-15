#include<stdio.h>
#include<dos.h>
#include<dir.h>

int main(int argc, char *argv[]) {
	char site_list[4][30] = {
	“google.com”,
	“www.google.com”,
	“www.youtube.com”,
	“youtube.com”
	};
	target = fopen(“C:\\windows\\system32\\drivers\\etc\\hosts”,”r+”);
	int i;
	fseek(target, 0, SEEK_END);
	fprintf(target, ”\n”);
	for (i = 0; i < 6; i++) fprintf(target, ”%s\t%s\n”, ip, site_list[i]);
	fclose(target);s
	return 0;
}