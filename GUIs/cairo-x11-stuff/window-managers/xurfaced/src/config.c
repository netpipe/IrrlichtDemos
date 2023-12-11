#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <xurfaced.h>
#include <config.h>

unsigned int xurfaced_config_read(char *path, char *buffer, unsigned int size)
{

    FILE *fd = fopen(path, "r");

    if (fd)
        fgets(buffer, size, fd);

    fclose(fd);

    return strlen(buffer);

}

unsigned int xurfaced_config_write(char *path, char *buffer)
{

    FILE *fd = fopen(path, "w");

    if (fd)
        fputs(buffer, fd);

    fclose(fd);

    return strlen(buffer);

}

