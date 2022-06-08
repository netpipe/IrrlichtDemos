#include <stdio.h>
#include <stdlib.h>

#include <string.h>

int
main(int argc, char **argv) {
  char answer[100], *p;
  char *ptr;

  printf("Type something:\n");

  if(fgets(answer, sizeof(answer), stdin) == NULL) {
    printf("EOF\n");
  }

  printf("You typed \"%s\"\n", answer);

  if((p = strchr(answer, '\n')) != NULL) {
    *p = '\0';
  }

  printf("You typed \"%s\"\n", answer);

  exit(0);
}

