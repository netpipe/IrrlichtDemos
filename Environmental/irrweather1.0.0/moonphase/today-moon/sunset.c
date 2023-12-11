/*
** Sunset.c - computes the sunset and sends the output to stdout
*/

#include <stdio.h>

main(int argc, char *argv[])
{
  int sunrh, sunrm, sunsh, sunsm;

  sun(&sunrh, &sunrm, &sunsh, &sunsm);

  printf("%02d:%02d\n", sunsh, sunsm);

  return(0);
}
