#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../inc/utility.h"

bool checkUserInputOptionMenu()
{
    return true;
}

static int getLine(char *prmpt, char *buffer, size_t sz)
{
    int ch, extra;

    if (sz < 2)
    {
        return SMALL_BUFF;
    }

    if (prmpt != NULL) 
    {
        printf ("%s", prmpt);
    
        fflush (stdout);
    }

    if (fgets (buffer, sz, stdin) == NULL)
    {
        return NO_INPUT;
    }

    size_t len = strlen(buffer);

    if (len < 1)
    {
        return NO_INPUT;
    }

    if (buffer[len - 1] != '\n') 
    {
        extra = 0;
    
        while (((ch = getchar()) != '\n') && (ch != EOF))
        {
            extra = 1;
        }
        return (extra == 1) ? TOO_LONG : OK;
    }

    buffer[len - 1] = '\0';

    return OK;
}