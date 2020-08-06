#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/parse.h"

int linecount(char *file) {
	int c;
	FILE *fp; 
	int count = 0;
	fp = fopen(file, "r"); 

	for(c = getc(fp); c != EOF; c = getc(fp)) 
		if(c == '\n')
			count++;

	fclose(fp);
	return count;
}

char *getfield(char *line, int num) {
	char *tok;
	for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ","))
		if (!--num)
		    return(tok);
	return NULL;
}
