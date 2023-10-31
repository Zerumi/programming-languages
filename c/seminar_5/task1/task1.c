#define print_var(x) printf(#x " is %d\n", x )

#define constval 41

#include <stdio.h>

int main (int argc, char* argv[]) {
	

	int x = 0;
	print_var(x);

	print_var(42);

	print_var(constval);


	return 0;

}
