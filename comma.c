#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csvparser/parse.h"

int main(int argc, char *argv[])
{
    //Reading configuration file
    FILE* constream = fopen("config.csv", "r");
    char conline[1024];
    char* ceditor;
    char* cgit;
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
	if (!strcmp(prop,"git"))
	{
	    cgit = val;
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
    if (strcmp(argv[1],"-l") == 0 && argc == 5)
    {
	char* newobj = malloc(22 + strlen(argv[2]) + strlen(argv[3]) + strlen(argv[4]));
	strcpy(newobj, argv[2]);
	strcat(newobj, ",");
	strcat(newobj, argv[3]);
	strcat(newobj, ",");
	strcat(newobj, argv[4]);
	strcat(newobj, "\n");
	FILE* objfile = fopen("objects.csv", "a");
	fprintf(objfile, newobj);
	printf("Created new object: %s", newobj);

	free(newobj);
    }

    //Unload
    else if (strcmp(argv[1],"-u") == 0 && argc == 3)
    {
	FILE* fileread = fopen("objects.csv", "r");
	FILE* tmpfilewrite = fopen("tmpobjects.csv", "w");
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int match = 0;

	if (fileread == NULL)
	    exit(EXIT_FAILURE);
	
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

	if(!match) {
	    fprintf(stderr, "Invalid object\n");
	    return(1);
	}
	
	while((read = getline(&line, &len, fileread)) != -1) {
	    char* linecheck = malloc(strlen(line) - 1);
	    strcpy(linecheck, line);
	    strtok(linecheck, "\n");
	    if(strcmp(linecheck, delobj) == 0) {
		printf("Match for: %s, line: %s\n", delobj, linecheck);
	    }
	    else {
		fprintf(tmpfilewrite, line);
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

    //Git
    else if (strcmp(argv[1],"-g") == 0)
    {
	strtok(cgit, "\n");
	if (strcmp(cgit,"true") == 0)
	{
	    printf("Git is enabled\n");
	}
	else
	{
	    fprintf(stderr, "ERROR: Git not enabled\n");
	    return(1);
	}
    }

    //Edit
    else if (argc == 2)
    {
	char* editpath;
	for (int line = 0; line < objidx; line++)
	{
	    strtok(objs[line][2], "\n");
	    if (strcmp(objs[line][2], argv[1]) == 0)
	    {
		editpath = objs[line][0];
	    }

	}
	if (strcmp(editpath,"") == 0)
	{
	    fprintf(stderr, "ERROR: Invalid arguments\n");
	    return(1);
	}
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
	fprintf(stderr, "ERROR: Invalid arguments\n");
	return(1);
    }
}
