#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void argerr() {
    fprintf(stderr, "ERROR: Invalid arguments\n");
    free(dir);
    exit(1);
}

void direrr() {
    fprintf(stderr, "ERROR: Directory error (most likely not found)\n");
    free(dir);
    exit(1);
}

void fileerr() {
    fprintf(stderr, "ERROR: File error (most likely not found)\n");
    free(dir);
    exit(1);
}

void objerr() {
    fprintf(stderr, "ERROR: Object error (most likely not found)\n");
    free(dir);
    exit(1);
}
