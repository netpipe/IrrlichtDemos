/*
** Sunrise.c - computes the sunrise and sends the output to stdout
*/

#include <stdio.h>

int
main(int argc, char *argv[])
{
  int sunrh, sunrm, sunsh, sunsm;

  sun(&sunrh, &sunrm, &sunsh, &sunsm);

  printf("%02d:%02d\n", sunrh, sunrm);

  return(0);
}
