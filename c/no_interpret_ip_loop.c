#include <stdio.h>

int main (int argc, char *argv[]) {
	unsigned int iterator;
	int ipStart[] ={192,168,0,100};
	int ipEnd[] = {192,168,10,100};

	unsigned int startIP= (
	ipStart[0] << 24 |
	ipStart[1] << 16 |
	ipStart[2] << 8 |
	ipStart[3]);

	unsigned int endIP= (
	ipEnd[0] << 24 |
	ipEnd[1] << 16 |
	ipEnd[2] << 8 |
	ipEnd[3]);

	for (iterator=startIP; iterator < endIP; iterator++) {
		printf (" %d.%d.%d.%d\n",
		(iterator & 0xFF000000) >> 24,
		(iterator & 0x00FF0000) >> 16,
		(iterator & 0x0000FF00) >> 8,
		(iterator & 0x000000FF)
		);
	}

	return 0;
}