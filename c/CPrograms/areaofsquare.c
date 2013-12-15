#include <stdio.h>
main() {
	int a, s;
	printf("Enter length of side: ");
	scanf("%d", &s); //store length in s
	a = s * s; //calculate area; store in a
	printf("\nArea of square is %d\n", a);
}