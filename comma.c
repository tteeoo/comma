/* COnfiguration Micro MAnager (comma) writen by Theo Henson
 *
 * gcc -o comma comma.c csvparser/parsefuncs.c errors/errorfuncs.c -I.
 *
 * TODO:
 * - Remove categories
 * - Toggle color
 * - Help option
 * - Makefile
 * - AUR
 * - ??????
 * - Profit
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "csvparser/parse.h"
#include "errors/error.h"

char* dir;
char* confname;
char* objname;
char* tmpname;

int main(int argc, char *argv[]) {
    //Initialize dir variable and file names
    dir = calloc(sizeof(getenv("HOME")) + 17, 1);
    strcpy(dir, getenv("HOME"));
    strcat(dir, "/.config/comma/");

    confname = calloc(sizeof(dir) + 20, 1);
    strcpy(confname, dir);
    strcat(confname, "config.csv");

    objname = calloc(sizeof(dir) + 20, 1);
    strcpy(objname, dir);
    strcat(objname, "objects.csv");

    tmpname = calloc(23, 1);
    strcpy(tmpname, "/tmp/comma_objects.csv");

    //Check if files exist and handle if not
    DIR* checkdir = opendir(dir);
    if(!checkdir) {
	mkdir(dir, 0700);
	closedir(checkdir);
    }
    if(access(confname, F_OK) == -1) {
	FILE* newconf = fopen(confname, "w");
	fprintf(newconf, "editor,vi\ncolor,true\n");
	fclose(newconf);
	printf("No config file detected, created new one (%s)\nThe default editor is currently vi, edit the config file to change it.\n", confname);
    }
    if(access(objname, F_OK) == -1) {
	FILE* newobj = fopen(objname, "w");
	fprintf(newobj, "~/.config/comma/config.csv,comma,c\n");
	fclose(newobj);
    }

    //Read config file
    FILE* constream = fopen(confname, "r");
    char conline[1024];
    char* ceditor;
    char* color;

    while(fgets(conline, 1024, constream)) {
        char* tmp = strdup(conline);
	char* prop = getfield(tmp, 1);
	tmp = strdup(conline);
	char* val = getfield(tmp, 2);
	tmp = strdup(conline);

	if(!strcmp(prop, "editor")) {
	    ceditor = val;
	}
	if(!strcmp(prop, "color")) {
	    color = val;
	}

	free(tmp);
    }
    fclose(constream);

    //Read object file
    FILE* objstream = fopen(objname, "r");
    char* objs[linecount(objname)][3];
    char objline[1024];
    int objidx = 0;
    while(fgets(objline, 1024, objstream)) {
        char* tmp = strdup(objline);
	char* path = getfield(tmp, 1);
	tmp = strdup(objline);
	char* cat = getfield(tmp, 2);
	tmp = strdup(objline);
	char* nick = getfield(tmp, 3);
	tmp = strdup(objline);

	objs[objidx][0] = path;
	objs[objidx][1] = cat;
	objs[objidx][2] = nick;

        free(tmp);
	objidx++;
    }
    fclose(objstream);

    //List (No arguments specified)
    if(argc == 1) {
	printf("%-25s%-25s%s", "Path", "Category",  "Nickname\n");
	for(int line = 0; line < objidx; line++) {
	    for(int type = 0; type < 3; type++) {
		strtok(objs[line][type], "\n");
		if(line % 2 == 0) {
		    printf("%s%s%-25s", "\033[47m", "\033[30m", objs[line][type]);
		}
		else {
		    printf("%s%s%-25s", "\033[40m", "\033[37m", objs[line][type]);
		}
	    }
	    printf("%s\n", "\033[0m");
	}
	success();
    }

    //Load
    if(strcmp(argv[1],"-l") == 0) {
	if(argc != 5)
	    argerr();

	char* newobj = malloc(22 + strlen(argv[2]) + strlen(argv[3]) + strlen(argv[4]));
	strcpy(newobj, argv[2]);
	strcat(newobj, ",");
	strcat(newobj, argv[3]);
	strcat(newobj, ",");
	strcat(newobj, argv[4]);
	strcat(newobj, "\n");
	FILE* objfile = fopen(objname, "a");
	fprintf(objfile, "%s", newobj);
	
	fclose(objfile);
	free(newobj);
	success();
    }

    //Unload
    else if(strcmp(argv[1],"-u") == 0) {
	if(argc != 3)
	    argerr();

	FILE* fileread = fopen(objname, "r");
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	int match = 0;
	
	char* delobj;

	for(int i=0; i < objidx; i++) {
	    strtok(objs[i][2], "\n");
	    if(strcmp(objs[i][2],argv[2]) == 0) {
		delobj = malloc(3 + strlen(objs[i][0]) + strlen(objs[i][1]) + strlen(objs[i][2]));
		strcpy(delobj, objs[i][0]);
		strcat(delobj, ",");
		strcat(delobj, objs[i][1]);
		strcat(delobj, ",");
		strcat(delobj, objs[i][2]);
		match = 1;
		break;
	    }
	}

	if(!match)
	    objerr();
	
	FILE* tmpfilewrite = fopen(tmpname, "w");
	
	while((read = getline(&line, &len, fileread)) != -1) {
	    char* linecheck = malloc(strlen(line) - 1);
	    strcpy(linecheck, line);
	    strtok(linecheck, "\n");
	    if(strcmp(linecheck, delobj) == 0) {}
	    else {
		fprintf(tmpfilewrite, "%s", line);
	    }
	}

	free(delobj);
	fclose(tmpfilewrite);
	fclose(fileread);
	FILE* tmpfileread = fopen(tmpname, "r");
	FILE* filewrite = fopen(objname, "w");
	char ch;

	while((ch = fgetc(tmpfileread)) != EOF)
	    fputc(ch, filewrite);

	fclose(filewrite);
	fclose(tmpfileread);
	remove(tmpname);
	
	if(line)
	    free(line);

	success();
    }

    //Edit
    else if(argc == 2) {
	char* editpath = NULL;
	for(int line = 0; line < objidx; line++) {
	    strtok(objs[line][2], "\n");
	    if (strcmp(objs[line][2], argv[1]) == 0) {
		editpath = objs[line][0];
	    }
	}

	if(editpath == NULL)
	    objerr();

	strtok(ceditor, "\n");
	char* command = malloc(2 + strlen(ceditor) + strlen(editpath));
	strcpy(command, ceditor);
	strcat(command, " ");
	strcat(command, editpath);
	system(command);

	free(command);
	success();
    }
    else {
	argerr();
    }
}
