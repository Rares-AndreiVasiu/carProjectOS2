#ifndef H_UTILITY
#define H_UTILITY

#include <stdbool.h>
#include <time.h>

#define OK         0
#define NO_INPUT   1
#define TOO_LONG   2
#define SMALL_BUFF 3

#define LOW_BOUND 1
#define UPPER_BOUND_ENTRACE 6

#define MAX_LENGTH 4096

#define NO_THREADS 16

bool checkUserInputOptionMenu(char *buff, int low, int high, int *option);

int getLine(char *prmpt, char *buffer, size_t sz);

int getUserOption(int low, int high);

void mySleep(unsigned duration);

char *encrypt(char * pswd, int shift);

char *decrypt(char *encrypted, int shift);

void loginHandler();

void clearScreen();

int getAccountFromCSV(const char *filename, char *email, char *password);

void extractEmailAndPassword(const char *csvLine, char *email, char *password);

void removeNexline(char *line);

void rentalHandler();

void createAccountHandler();

void addCarHandler();

void repairCarHandler();

#endif