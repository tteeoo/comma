#include "dir.h"
#include <string.h>

int strinarr(char* str, char* arr[])
{
    int i = 0;
    while(arr[i]) {
        if(strcmp(arr[i], str) == 0) {
	    return(1);
        }
        i++;
    }
    return(0);
}
