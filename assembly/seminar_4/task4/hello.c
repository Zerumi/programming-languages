#include "print_file.h"
#include<stdio.h>
/* hello.c */
int main() {
    char str[80];
    print_string("Please, enter filename: ");
    int r = scanf("%79s", str);
    if (r >= 0) {    
    	print_file(str);
    }
    return r;
}
