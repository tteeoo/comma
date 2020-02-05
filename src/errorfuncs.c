#include "../include/error.h"
#include <stdio.h>
#include <stdlib.h>

void argerr() {
    fprintf(stderr, "ERROR: Invalid arguments\n");
    free(dir);
    free(confname);
    free(objname);
    free(tmpname);
    exit(1);
}

void objerr() {
    fprintf(stderr, "ERROR: Object not loaded\n");
    free(dir);
    free(confname);
    free(objname);
    free(tmpname);
    exit(1);
}

void success() {
    free(dir);
    free(confname);
    free(objname);
    free(tmpname);
    exit(0);
}
