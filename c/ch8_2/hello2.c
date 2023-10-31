/* This is a comment. The next line has a preprocessor directive */
#include /* test comment :) */ <stdio.h>

/* `main` is the entry point for the program, like _start in assembly
 * Actually, the hidden function _start is calling `main`.
 * `main` returns the `return code` which is then given to the `exit` system
 * call.
 * The `void` keyword instead of argument list means that `main` accepts no
 * arguments */

const int global_var1 = 42;
int global_var2 = global_var1 + 1295;

int main(void) {
	int x = /* test test */ 43;
	int y;
	y = x - 1;
	/* Calling a standard function `printf` with three arguments.
    	 * It will print 'Hello, world! y=42 x=43
    	 * All %d  will be replaced by the consecutive arguments */
	printf("girls sometimes may be beautiful... y=%d, x=%d\n", y, x);
	printf("testing global vars: 1:%d, 2:%d\n", global_var1, global_var2);
	return 0;
}
