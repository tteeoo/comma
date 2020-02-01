/* COnfiguration Micro MAnager (comma) writen by Theo Henson
 *
 * gcc -o comma comma.c csvparser/parsefuncs.c errors/errorfuncs.c -I.
 *
 * TODO:
 * - Git functionality
 * - Read from ~/.config/
 * - Clean up code
 * - Makefile
 * - AUR
 * - ??????
 * - Profit
 *
 * WARNING: This code is only partially functional
 *
 */

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "csvparser/parse.h"
#include "dirtools/dir.h"
#include "errors/error.h"

char* dir;

int main(int argc, char *argv[])
{
    dir = malloc(sizeof(getenv("HOME")) + 13);
    strcpy(dir, getenv("HOME"));
    strcat(dir, ".config/comma/");

    FILE* constream = fopen("config.csv", "r");
    char conline[1024];
    char* ceditor;
    char* cdir;
    char* crepo;
    while (fgets(conline, 1024, constream))
    {
        char* tmp = strdup(conline);
	char* prop = getfield(tmp, 1);
	tmp = strdup(conline);
	char* val = getfield(tmp, 2);
	tmp = strdup(conline);

	if (!strcmp(prop,"editor"))
	{
	    ceditor = val;
	}
	if (!strcmp(prop,"dir"))
	{
	    cdir = val;
	}
	if (!strcmp(prop,"repo"))
	{
	    crepo = val;
	}

	free(tmp);
    }

    //Reading object file
    FILE* objstream = fopen("objects.csv", "r");
    char* objs[linecount("objects.csv")][3];
    char objline[1024];
    int objidx = 0;
    while (fgets(objline, 1024, objstream))
    {
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

    //List (No arguments specified)
    if (argc == 1)
    {
	printf("%-25s%-25s%s", "Path", "Category",  "Nickname\n");
	for(int line = 0; line < objidx; line++)
	{
	    for(int type = 0; type < 3; type++)
	    {
		strtok(objs[line][type], "\n");
		if (line % 2 == 0)
		{
		    printf("%s%s%-25s", "\033[47m", "\033[30m", objs[line][type]);
		}
		else
		{
		    printf("%s%s%-25s", "\033[40m", "\033[37m", objs[line][type]);
		}
	    }
	    printf("%s\n", "\033[0m");
	}
	return(0);
    }

    //Load
    if (strcmp(argv[1],"-l") == 0)
    {
	if(argc != 5)
	    argerr();

	char* newobj = malloc(22 + strlen(argv[2]) + strlen(argv[3]) + strlen(argv[4]));
	strcpy(newobj, argv[2]);
	strcat(newobj, ",");
	strcat(newobj, argv[3]);
	strcat(newobj, ",");
	strcat(newobj, argv[4]);
	strcat(newobj, "\n");
	FILE* objfile = fopen("objects.csv", "a");
	fprintf(objfile, "%s", newobj);
	printf("Created new object: %s", newobj);

	free(newobj);
    }

    //Unload
    else if (strcmp(argv[1],"-u") == 0)
    {
	if(argc != 3)
	    argerr();

	FILE* fileread = fopen("objects.csv", "r");
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	int match = 0;

	if (fileread == NULL)
	    fileerr();
	
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
	

	FILE* tmpfilewrite = fopen("tmpobjects.csv", "w");
	
	while((read = getline(&line, &len, fileread)) != -1) {
	    char* linecheck = malloc(strlen(line) - 1);
	    strcpy(linecheck, line);
	    strtok(linecheck, "\n");
	    if(strcmp(linecheck, delobj) == 0) {
		printf("Match for: %s, line: %s\n", delobj, linecheck);
	    }
	    else {
		fprintf(tmpfilewrite, "%s", line);
	    }
	}

	free(delobj);
	fclose(tmpfilewrite);
	fclose(fileread);
	FILE* tmpfileread = fopen("tmpobjects.csv", "r");
	FILE* filewrite = fopen("objects.csv", "w");
	char ch;
	while ((ch = fgetc(tmpfileread)) != EOF)
	    fputc(ch, filewrite);

	fclose(filewrite);
	fclose(tmpfileread);
	remove("tmpobjects.csv");
	
	if (line)
	    free(line);
    }

    //Dir
    else if (strcmp(argv[1],"-d") == 0)
    {
	if (argc != 3 || (strcmp(argv[2], "pull") != 0 && strcmp(argv[2], "push") != 0))
	    argerr();

	strtok(cdir, "\n");

	//Push
	if (strcmp(argv[2], "push") == 0)
	{
	    printf("Push\n");
	}
	
	//Pull
	else
	{
	    printf("Pull\n");
	    struct dirent *de;
  
	    DIR *dr = opendir(cdir); 
  
	    if (dr == NULL)
		direrr();
  
	    while ((de = readdir(dr)) != NULL) 
		printf("%s\n", de->d_name); 
  
	    closedir(dr);
	    if(strinarr("f", objs[0]))
		printf("q");
	}
    }

    //Edit
    else if (argc == 2)
    {
	char* editpath = NULL;
	for (int line = 0; line < objidx; line++)
	{
	    strtok(objs[line][2], "\n");
	    if (strcmp(objs[line][2], argv[1]) == 0)
	    {
		editpath = objs[line][0];
	    }

	}

	if (editpath == NULL)
	    objerr();

	strtok(ceditor, "\n");
	char* command = (char *) malloc(2 + strlen(ceditor)+ strlen(editpath) );
	strcpy(command, ceditor);
	strcat(command, " ");
	strcat(command, editpath);
	printf("RUNNING: %s \n", command);
	system(command);
	free(command);
    }
    else
    {
	argerr();
    }
    free(dir);
}
