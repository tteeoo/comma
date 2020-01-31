#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void argerr() {
    fprintf(stderr, "ERROR: Invalid arguments\n");
    free(dir);
    exit(1);
}

void giterr() {
    fprintf(stderr, "ERROR: Git not enabled\n");
    free(dir);
    exit(1);
}
