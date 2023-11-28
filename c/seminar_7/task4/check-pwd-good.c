/* check-pwd.c */
/* pass check by this command: echo -n -e "\x72\x72\x72\x72\x72\x72\x72\x72\x72\x72\x1" | ./check-pwd */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int check_password(FILE *f, const char *password) {
  char buffer[10];
  int okay = 0;
  fscanf(f, "%s", buffer);
  // printf("%zu", fscanf(f, "%s", buffer)); // 1
  
  if (strcmp(buffer, password) == 0)
    okay = 1;
  else okay = 0;

  return okay;
}

int main(int argc, char **argv) {
  if (check_password(stdin, "password"))
    puts("Access granted.");
  else
    puts("Wrong password.");
}