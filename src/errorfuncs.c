#include "../include/error.h"
#include <stdio.h>
#include <stdlib.h>

void argerr() {
    fprintf(stderr, "ERROR: Invalid arguments\n");
    printf("Usage:\n	comma [<-l | --load> <nickname> <description> <filepath>] | [<-u | --unload> <nickname>] | [<-h | --help>] | [nickname]\n");
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
