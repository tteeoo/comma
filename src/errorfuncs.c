#include <stdio.h>
#include <stdlib.h>

#include "../inc/error.h"

void argerr() {
	fprintf(stderr, "comma: error: invalid arguments\n");
	printf("comma: usage:\n\tcomma [<-l | --load> <nickname> <description> <filepath>] | [<-u | --unload> <nickname>] | [<-h | --help>] | [nickname]\noptions:\n\t-l, --load: loads an object with specified nickname, description, and path\n\n\t-u, --unload: unloads an object with specified nickname\n\n\t-h --help: displays this message\n\nwithout any options comma will list all of your tracked configuration files (objects)\nproviding the nickname of a loaded object as the only argument will open the file in your specified editor\n");
	free(dir);
	free(confname);
	free(objname);
	free(tmpname);
	exit(1);
}

void objerr() {
	fprintf(stderr, "comma: error: object not loaded\n");
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
