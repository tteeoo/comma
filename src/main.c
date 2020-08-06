// COnfiguration Micro MAnager (comma) writen by Theo Henson
//
// MIT License

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../inc/parse.h"

char *objname;

void err(char *name) {
	free(objname);
	fprintf(stderr, "comma: error: %s\n", name);
	exit(1);
}

int main(int argc, char *argv[]) {

	// Initialize dir variable and file names
	char *dir = malloc(sizeof(getenv("HOME")) + 17);
	char *home = getenv("HOME");
	if (!home)
		err("HOME environment variable not set");
	strcpy(dir, getenv("HOME"));
	strcat(dir, "/.config/comma/");

	char *confname = malloc(sizeof(dir) + 20);
	strcpy(confname, dir);
	strcat(confname, "config.csv");

	objname = malloc(sizeof(dir) + 20);
	strcpy(objname, dir);
	strcat(objname, "objects.csv");

	// Check if files exist and handle if not
	DIR *checkdir = opendir(dir);
	if (!checkdir) {
		mkdir(dir, 0700);
		closedir(checkdir);
	}
	free(dir);
	if (access(confname, F_OK) == -1) {
		FILE *newconf = fopen(confname, "w");
		fprintf(newconf, "editor,vi\ncolor,true\n");
		fclose(newconf);
		printf("comma: no config file detected, created new one (%s)\ncomma: the default editor is currently vi, edit the config file to change it\n", confname);
	}
	if (access(objname, F_OK) == -1) {
		FILE *newobj = fopen(objname, "w");
		fprintf(newobj, "c,comma config file,~/.config/comma/config.csv\n");
		fclose(newobj);
	}

	// Read config file
	FILE *constream = fopen(confname, "r");
	free(confname);
	char conline[1024];
	char *ceditor;
	char *color;

	while (fgets(conline, 1024, constream)) {
		char *tmp = strdup(conline);
		char *prop = getfield(tmp, 1);
		tmp = strdup(conline);
		char *val = getfield(tmp, 2);
		tmp = strdup(conline);

		if (!strcmp(prop, "editor"))
			ceditor = val;
		if (!strcmp(prop, "color"))
			color = val;

		free(tmp);
	}
	fclose(constream);

	// Read object file
	FILE *objstream = fopen(objname, "r");
	char *objs[linecount(objname)][3];
	char objline[1024];
	int objidx = 0;
	while (fgets(objline, 1024, objstream)) {
		char *tmp = strdup(objline);
		char *nick = getfield(tmp, 1);
		tmp = strdup(objline);
		char *desc = getfield(tmp, 2);
		tmp = strdup(objline);
		char *path = getfield(tmp, 3);
		tmp = strdup(objline);

		objs[objidx][0] = nick;
		objs[objidx][1] = desc;
		objs[objidx][2] = path;

		free(tmp);
		objidx++;
	}
	fclose(objstream);

	// List (No arguments specified)
	if (argc == 1) {
		strtok(color, "\n");
		printf("%-25s%-25s%s\n", "nickname", "description",  "path");
		for (int line = 0; line < objidx; line++) {
			for (int type = 0; type < 3; type++) {
				strtok(objs[line][type], "\n");
				if(strcmp(color,"true") == 0) {
					if(line % 2 == 0) {
						printf("%s%s%-25s", "\033[47m", "\033[30m", objs[line][type]);
					} else {
						printf("%s%s%-25s", "\033[40m", "\033[37m", objs[line][type]);
					}
				} else {
					printf("%-25s", objs[line][type]);
				}
			}
			printf("%s\n", "\033[0m");
		}
		free(objname);
		return 0;
	}

	// Load
	if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--load") == 0) {
		if(argc != 5)
			err("invalid arguments");

		char *newobj = malloc(22 + strlen(argv[2]) + strlen(argv[3]) + strlen(argv[4]));
		strcpy(newobj, argv[2]);
		strcat(newobj, ",");
		strcat(newobj, argv[3]);
		strcat(newobj, ",");
		strcat(newobj, argv[4]);
		strcat(newobj, "\n");
		FILE *objfile = fopen(objname, "a");
		fprintf(objfile, "%s", newobj);
		
		fclose(objfile);
		free(newobj);
		free(objname);
		return 0;
	}

	// Unload
	else if (strcmp(argv[1], "-u") == 0 || strcmp(argv[1], "--unload") == 0) {
		if (argc != 3)
			err("invalid arguments");

		FILE *fileread = fopen(objname, "r");
		char *line = NULL;
		size_t len = 0;
		ssize_t read;
		int match = 0;
		
		char *delobj;

		for (int i=0; i < objidx; i++) {
			strtok(objs[i][0], "\n");
			if (strcmp(objs[i][0],argv[2]) == 0) {
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

		if (!match)
			err("object not loaded");
	
		char *tmpname = malloc(23);
		strcpy(tmpname, "/tmp/comma_objects.csv");

		FILE *tmpfilewrite = fopen(tmpname, "w");
	
		while ((read = getline(&line, &len, fileread)) != -1) {
			char *linecheck = malloc(strlen(line) - 1);
			strcpy(linecheck, line);
			strtok(linecheck, "\n");
			if (strcmp(linecheck, delobj) != 0)
				fprintf(tmpfilewrite, "%s", line);
		}

		free(delobj);
		fclose(tmpfilewrite);
		fclose(fileread);
		FILE *tmpfileread = fopen(tmpname, "r");
		FILE *filewrite = fopen(objname, "w");
		char ch;

		while ((ch = fgetc(tmpfileread)) != EOF)
			fputc(ch, filewrite);

		fclose(filewrite);
		fclose(tmpfileread);
		remove(tmpname);
		free(tmpname);
	
		if(line )
			free(line);

		free(objname);
		return 0;
	}

	// Help
	else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		printf("comma: usage:\n\tcomma [<-l | --load> <nickname> <description> <filepath>] | [<-u | --unload> <nickname>] | [<-h | --help>] | [nickname]\noptions:\n\t-l, --load: loads an object with specified nickname, description, and path\n\n\t-u, --unload: unloads an object with specified nickname\n\n\t-h --help: displays this message\n\nwithout any options comma will list all of your tracked configuration files (objects)\nproviding the nickname of a loaded object as the only argument will open the file in your specified editor\n");
	}

	// Edit
	else if (argc == 2) { 
		char *editpath = NULL;
		for(int line = 0; line < objidx; line++) {
			strtok(objs[line][0], "\n");
			if(strcmp(objs[line][0], argv[1]) == 0) {
				editpath = objs[line][2];
			}
		}

		if (editpath == NULL)
			err("object not loaded");

		strtok(ceditor, "\n");
		char *command = malloc(2 + strlen(ceditor) + strlen(editpath));
		strcpy(command, ceditor);
		strcat(command, " ");
		strcat(command, editpath);
		system(command);

		free(command);
		free(objname);
		return 0;
	} else {
		err("invalid arguments");
	}

	free(objname);
	return 0;
}
