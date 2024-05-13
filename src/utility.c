#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

#include "../inc/utility.h"

bool checkUserInputOptionMenu(char *buff, int low, int high, int *option)
{
    // printf("Hello there!\n");

    long num;

    char *endptr;

    // we reset the error number
    errno = 0;

    num = strtol(buff, &endptr, 10);

    // printf("we have a nasty conversion here!\n");

    if(errno == ERANGE)
    {
        // out of range for the long type 
        return false;
    }

    // printf("errno\n");

    if(*endptr && ((*endptr) != '\n'))
    {
        // *endptr is neither end of string nor newline,
        // so we didn't convert the "whole" input given in buffer

        return false;
    }

    // printf("check whole input\n");

    if((num > INT_MAX || num < INT_MIN) || (num < low || num > high))
    {
        // num is not in the boundaries of int data type
        // printf("check bounds\n");

        return false;
    }

    // printf("check bounds fr\n");

    // we write the result through th pointer passed argument

    // *option = (int) num;

    // printf("long to int: %ld\n", num);

    if(option != NULL)
    {
        // printf("Option different than null!\n");

        *option = (int) num;
    }
    
    // printf("Hello there final number!\n");

    return true;
}

int getLine(char *prmpt, char *buffer, size_t sz)
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

int getUserOption(int low, int high)
{
    char buffer[MAX_LENGTH];

    bool flagValidBuffer = false;

    bool flagValidNumber = false;
    
    int temp = 0;

    int *option = &temp;

    //user option retrieval

    while(!flagValidBuffer && !flagValidNumber)
    {
        do
        {
            int rc = getLine("Enter the option: ", buffer, MAX_LENGTH);

            if(rc == NO_INPUT)
            {
                //we got no input
                flagValidBuffer = false;
            }

            if(rc == TOO_LONG)
            {
                //input too long
                flagValidBuffer = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagValidBuffer = true;
            }

        } while (!flagValidBuffer);

        // printf("We have a valid string!\n");

        //now we have a valid length buffer    

        if(checkUserInputOptionMenu(buffer, low, high, option))
        {
            // printf("we have a valid number!\n");

            flagValidNumber = true;
        }
        else
        {
            flagValidNumber = false;
        }
    }

    return (*option);
}

void mySleep(unsigned duration)
{
    time_t start = time(NULL);

    double end = duration;

    time_t now;

    do
    {
        now = time(NULL);

    }while(difftime(now, start) < end);
}

char *encrypt(char* pswd, int shift)
{
    char *s = (char *)malloc(sizeof(char) * strlen(pswd));

    if(!s)
    {
        perror("malloc");

        return NULL;
    }

    for (int i = 0; i < strlen(pswd); ++ i)
    {
        int value;

        char c2 = pswd[i];

        if ('a' <= c && c <= 'z')
        {
            value = pswd[i] - 'a';

            value = (value + shift) % 26;

            c2 = value + 'a';
        }
        else
        {
            if ('A' <= pswd[i] && pswd[i] <= 'Z')
            {
                value = pswd[i] - 'A';

                value = (value + shift) % 26;

                c2 = value + 'A';
            }
        }

        s[i] = c2;
    }

    s[strlen(pswd)] = '\0';

    return s;
}

char *decrypt(char *encrypted, int shift)
{
    char *s = (char *)malloc(sizeof(char) * strlen(pswd));

    if(!s)
    {
        perror("malloc");

        return NULL;
    }

    for (int i = 0; i < strlen(pswd); ++ i)
    {
        int value;
        char c2 = c;

        if ('a' <= c && c <= 'z')
        {
            value = c - 'a';

            value = (value + 26 - n) % 26;

            c2 = value + 'a';
        }
        else
        {
            if ('A' <= c && c <= 'Z')
            {
                value = c - 'A';

                value = (value + 26 - n) % 26;

                c2 = value + 'A';
            }
        }

        s += c2;
    }

    return s;
}