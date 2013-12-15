#include <stdio.h>

int main(void) {
	int counter;
	unsigned int ip[] = {192, 168, 0, 0};

	for (counter = 0; counter < 1000; ++counter) {
		ip[3] = (++ ip[3] % 256);
		if (!ip[3]) ip[2] = (++ ip[2] % 256);
		printf("%u:%u:%u:%u\n", ip[0], ip[1], ip[2], ip[3]);
	}

	return 0;
}