#include <stdio.h>

int main(void) {
	int b = 129;
	char k = (char) b; /* ??? */
	printf("%d -> %c\n", b, k);
	return 0;
}
