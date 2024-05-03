#ifndef H_UTILITY
#define H_UTILITY

#define OK         0
#define NO_INPUT   1
#define TOO_LONG   2
#define SMALL_BUFF 3

bool checkUserInputOptionMenu();

static int getLine (char *prmpt, char *buff, size_t sz);

#endif