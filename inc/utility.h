#ifndef H_UTILITY
#define H_UTILITY

#include <stdbool.h>
#include <time.h>

#define OK         0
#define NO_INPUT   1
#define TOO_LONG   2
#define SMALL_BUFF 3

#define LOW_BOUND 1
#define UPPER_BOUND_ENTRACE 3

#define MAX_LENGTH 4096

bool checkUserInputOptionMenu(char *buff, int low, int high, int *option);

int getLine(char *prmpt, char *buffer, size_t sz);

int getUserOption(int low, int high);

void mySleep(unsigned duration);

char *encrypt(char * pswd, int shift);

char *decrypt(char *encrypted, int shift);

#endif