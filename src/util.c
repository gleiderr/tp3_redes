#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "util.h"

void die(char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}