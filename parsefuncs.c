#include <parse.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int linecount(char* file)
{
    FILE *fp; 
    int count = 0;
    int c;
    fp = fopen(file, "r"); 
    for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n')
            count = count + 1;
    fclose(fp);
    return(count);
}

char* getfield(char* line, int num)
{
    char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ","))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}
