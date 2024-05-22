#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "../inc/utility.h"
#include "../inc/menu.h"

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

char *encrypt(char *pswd, int shift) 
{
    char *s = (char *)malloc(strlen(pswd) + 1);

    if (!s) 
    {
        perror("malloc");
    
        return NULL;
    }

    for (int i = 0; pswd[i] != '\0'; ++i) 
    {
        char c2 = pswd[i];
    
        int value;

        if (islower(pswd[i]))
        {
            value = (pswd[i] - 'a' + shift) % 26;
        }
        else 
        {
            if (isupper(pswd[i]))
            {
                value = (pswd[i] - 'A' + shift) % 26;
            }
            else
            {
                value = 0; // Handle non-letter characters
            }
        }

        c2 = islower(pswd[i]) ? value + 'a' : value + 'A';

        s[i] = c2;
    }

    s[strlen(pswd)] = '\0';

    return s;
}

char *decrypt(char *encrypted, int shift)
{
    char *decrypted = (char *)malloc(strlen(encrypted) + 1);

    if (!decrypted)
    {
        perror("malloc");
    
        return NULL;
    }

    for (int i = 0; encrypted[i] != '\0'; ++i)
    {
        char c = encrypted[i];

        if (islower(c) || isupper(c))
        {
            int offset = isupper(c) ? 'A' : 'a';

            int value = c - offset;
            
            value = (value + 26 - shift) % 26;
            
            decrypted[i] = value + offset;
        }
        else
        {
            decrypted[i] = c; 
        }
    }

    decrypted[strlen(encrypted)] = '\0'; 

    return decrypted;
}

void removeNewline(char *line)
{
    char *newline = strchr(line, '\n');

    if(newline != NULL)
    {
        *newline = '\0';
    }
}

int getAccountFromCSV(const char *filename,  char *email,  char *password) 
{
    FILE *fp = fopen(filename, "r");

    if (!fp) 
    {
        perror("Error opening file");

        return -1;
    }

    char *line = NULL;

    size_t len = 0;
    
    int read;
    
    int linesRead = 0;

    while ((read = getline(&line, &len, fp)) != -1) 
    {
        removeNewline(line);

        printf("Line read: %s\n", line);

        if (linesRead == 0) 
        {
            ++linesRead;

            continue;
        }

        char *data = (char *) malloc(read + 1);

        if (!data) 
        {
            perror("Error allocating memory");

            fclose(fp);
            
            free(line);
            
            return -1;
        }

        strncpy(data, line, read);

        printf("here we have the data: %s\n", data);    

        data[read] = '\0';

        char em[256] = {0}; 

        char pswd[256] = {0};

        extractEmailAndPassword(data, em, pswd);

        char *decryptedPswd = (char *)malloc(sizeof(char) * strlen(password) + 1);

        if(!decryptedPswd)
        {
            perror("malloc failed!");

            return -1;
        }

        decryptedPswd = decrypt(pswd, 16);

        printf("email and password found in the csv: %s %s\n", em, pswd);

        printf("email and password from the user: %s %s\n", email, password);

        if(strcmp(decryptedPswd, password) == 0 && strcmp(email, em) == 0)
        {
            free(data);

            free(line);
            
            fclose(fp);
            
            return 0;
        }

        free(data);

        free(decryptedPswd);

        ++linesRead;
    }

    fclose(fp);

    free(line);
    
    return -1;
}

void extractEmailAndPassword(const char *csv_line, char *email, char *password) 
{
     // Ensure email and password are cleared before use
    email[0] = '\0';

    password[0] = '\0';

    // Use sscanf to parse the CSV line
    sscanf(csv_line, "%255[^,],%*[^,],%255s", email, password);
    // char line_copy[MAX_LENGTH]; 

    // strncpy(line_copy, csv_line, sizeof(line_copy));
    
    // line_copy[sizeof(line_copy) - 1] = '\0';

    // char *token = strtok(line_copy, ",");

    // if (token != NULL) 
    // {
    //     strncpy(email, token, MAX_LENGTH);
    
    //     email[MAX_LENGTH] = '\0'; 
    // }

    // token = strtok(NULL, ",");

    // token = strtok(NULL, ",");

    // if (token != NULL) 
    // {
    //    strncpy(password, token, MAX_LENGTH);
    
    //     password[MAX_LENGTH] = '\0';
    // }
}

void clearScreen()
{
    printf("\e[1;1H\e[2J");
}

void loginHandler()
{
    bool flag = false, flagEmail = false, flagPassword = false;

    char email[MAX_LENGTH], password[MAX_LENGTH];

    do
    { 
        clearScreen();
        
        loginMenu();
        
        do
        {
            int rc = getLine("Enter the email: ", email, MAX_LENGTH);

            if(rc == NO_INPUT)
            {
                //we got no input
                flagEmail = false;
            }

            if(rc == TOO_LONG)
            {
                //input too long
                flagEmail = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagEmail = true;
            }

        } while (!flagEmail);

        do
        {
            int rc = getLine("Enter the password: ", password, MAX_LENGTH);

            if(rc == NO_INPUT)
            {
                //we got no input
                flagPassword = false;
            }

            if(rc == TOO_LONG)
            {
                //input too long
                flagPassword = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagPassword = true;
            }

        } while (!flagPassword);

        printf("Email: %s, password: %s\n", email, password);

        flag = true;

        mySleep(1);

    } while (!flag);
    
    char filepath[] = "../resources/login.csv";

    if(getAccountFromCSV(filepath, email, password) == 0)
    {
        //user exists in database already
        printf("User already\n");
    }
    else 
    {
        //user does not exist in database
        printf("User does not exist\n");
    }
}

int writeAccountToCSV(char *filepath, char *username, char *email, char *password)
{
    int fd = open(filepath, O_WRONLY | O_APPEND | O_CREAT, 0777);

    if(fd < 0)
    {
        perror("file open error");

        return -1;
    }

    char line[MAX_LENGTH];

    char *encryptedPswd = (char *) malloc(sizeof(char) * strlen(password) + 1);

    if(!encryptedPswd)
    {
        perror("malloc failed!");

        close(fd);

        return -1;
    }

    encryptedPswd = encrypt(password, 16);

    snprintf(line, MAX_LENGTH - 1, "%s,%s,%s\n", email, username, encryptedPswd);

    ssize_t len = strlen(line);

    ssize_t written = write(fd, line, len);

    if(written < 0)
    {
        perror("file write error");

        close(fd);

        return -1;
    }

    if(close(fd) < 0)
    {
        perror("error closing file");

        return -1;
    }

    return 0;
}

void createAccountHandler()
{
    bool flag = false, flagUsername = false, flagEmail = false, flagPassword = false;

    char email[MAX_LENGTH], password[MAX_LENGTH], username[MAX_LENGTH];

    do
    {
        clearScreen();

        createAccountMenu();
        
        do
        {
            int rc = getLine("Enter the username: ", username, MAX_LENGTH - 1);

            if(rc == NO_INPUT)
            {
                // we got no input
                flagUsername = false;
            }

            if(rc == TOO_LONG)
            {
                // input too long
                flagUsername = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagUsername = true;
            }

        } while (!flagUsername);

        do
        {
            int rc = getLine("Enter the email: ", email, MAX_LENGTH - 1);

            if(rc == NO_INPUT)
            {
                // we got no input
                flagEmail = false;
            }

            if(rc == TOO_LONG)
            {
                // input too long
                flagEmail = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagEmail = true;
            }
            
        } while (!flagEmail);

        do
        {
            int rc = getLine("Enter the password: ", password, MAX_LENGTH - 1);

            if(rc == NO_INPUT)
            {
                // we got no input
                flagPassword = false;
            }

            if(rc == TOO_LONG)
            {
                // input too long
                flagPassword = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagPassword = true;
            }
            
        } while (!flagPassword);

        printf("Username: %s, Email: %s, password: %s\n", username, email, password);

        flag = true;

        mySleep(1);
        
    } while (!flag);
    
    char filepath[] = "../resources/login.csv";

    if(writeAccountToCSV(filepath, username, email, password) == 0)
    {
        printf("User created\n");
    }
    else
    {
        printf("User failure!\n");
    }
}

typedef struct 
{
    char **vinArray;
    int start;
    int end;
    char *vin;
    bool *found;
    pthread_mutex_t *found_mutex;

} ThreadData;

void *threadedSearchVIN(void *args)
{
    ThreadData *data = (ThreadData *) args;

    for(int i = data->start; i < data->end && !*(data -> found); ++ i)
    {
        if(strcmp(data -> vinArray[i], data->vin) == 0)
        {
            pthread_mutex_lock(data->found_mutex);

            *(data->found) = true;

            pthread_mutex_unlock(data->found_mutex);

            printf("We have found this car: %s\n", data->vin);
        }
    }

    return NULL;
}

void rentalHandler()
{
    char fileName[] = "../resources/rental.csv";

    char *line = NULL;

    char **vinArray = NULL;
    
    int numVins = 0;

    FILE* fd = fopen(fileName, "r");

    if (!fd) 
    {
        perror("error opening file");

        return;
    }

    size_t len = 0;
    
    int read;

    int linesRead = 0;

    while ((read = getline(&line, &len, fd)) != -1) 
    {
        removeNewline(line);

        printf("Line read: %s\n", line);

        if (linesRead == 0) 
        {
            ++ linesRead;

            continue;
        }

        char *data = (char *) malloc(read + 1);

        if (!data) 
        {
            perror("Error allocating memory");

            fclose(fd);
            
            free(line);
        }

        strncpy(data, line, read);

        printf("here we have the data: %s\n", data);    

        data[read] = '\0';

        char* token = strtok(data, ",");

        for (int i = 0; i < 2; ++i) 
        {
            token = strtok(NULL, ",");

            if (!token) 
            {
                perror("wrong csv format");

                fclose(fd);

                for(int j = 0; j < numVins; ++ j)
                {
                    free(vinArray[j]);
                }

                free(vinArray);
                
                return;
            }
        }

        char **temp = (char **) realloc(vinArray, (numVins + 1) * sizeof(char *));

        if(!temp)
        {
            perror("memory failure");

            fclose(fd);

            for(int j = 0; j < numVins; ++ j)
            {
                free(vinArray[j]);
            }

            free(vinArray);

            return;
        }

        vinArray = temp;

        vinArray[numVins] = (char *) malloc(strlen(token) + 1);

        if(!vinArray[numVins])
        {
            perror("malloc failed");

            fclose(fd);

            for(int j = 0; j < numVins; ++ j)
            {
                free(vinArray[j]);
            }

            free(vinArray);
        }

        strcpy(vinArray[numVins], token);

        ++numVins;

        ++linesRead;
    }

    bool flag = false, flagVin = false;

    char vin[MAX_LENGTH];

    do
    { 
        clearScreen();
        
        rentCarMenu();
        
        do
        {
            int rc = getLine("Enter the vin: ", vin, MAX_LENGTH);

            if(rc == NO_INPUT)
            {
                //we got no input
                flagVin = false;
            }

            if(rc == TOO_LONG)
            {
                //input too long
                flagVin = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagVin = true;
            }

        } while (!flagVin);

        printf("VIN: %s\n", vin);

        flag = true;

        mySleep(1);

    } while (!flag);

    bool flagFound = false;

    pthread_mutex_t found_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_t threads[NO_THREADS];

    ThreadData threadData[NO_THREADS];

    int workloadPerThread = numVins / NO_THREADS;

    for(int i = 0; i < NO_THREADS; ++ i)
    {
        threadData[i].vinArray = vinArray;

        threadData[i].start = i * workloadPerThread;

        if(i == NO_THREADS - 1)
        {
            threadData[i].end = numVins;
        }
        else
        {
            threadData[i].end = (i + 1) * workloadPerThread;
        }

        threadData[i].vin = vin;

        threadData[i].found = &flagFound;

        threadData[i].found_mutex = &found_mutex;

        pthread_create(&threads[i], NULL, threadedSearchVIN, &threadData[i]);
    }

    for(int i = 0; i < NO_THREADS; ++ i)
    {
        pthread_join(threads[i], NULL);
    }

    if(!flagFound)
    {
        printf("No car was fond!\n");
    }

    // Clean this garbage
    fclose(fd);

    for (int i = 0; i < numVins; ++ i) 
    {
        free(vinArray[i]);
    }

    free(vinArray);
}

int addCarToCSV(char *filepath, char *vin, char *model, char *manufacturer)
{
    int fd = open(filepath, O_WRONLY | O_APPEND | O_CREAT, 0777);

    if(fd < 0)
    {
        perror("file open error");

        return -1;
    }

    char line[MAX_LENGTH];

    snprintf(line, MAX_LENGTH - 1, "%s,%s,%s\n", model, manufacturer, vin);

    ssize_t len = strlen(line);

    ssize_t written = write(fd, line, len);

    if(written < 0)
    {
        perror("file write error");

        close(fd);

        return -1;
    }

    if(close(fd) < 0)
    {
        perror("error closing file");

        return -1;
    }

    return 0;
}

int addToRepairShopCSV(char *repairPath, char *vin, char *status)
{
    int fd = open(repairPath, O_WRONLY | O_APPEND | O_CREAT, 0777);

    if(fd < 0)
    {
        perror("file open error");

        return -1;
    }

    char line[MAX_LENGTH];

    snprintf(line, MAX_LENGTH - 1, "%s,%s\n", vin, status);

    ssize_t len = strlen(line);

    ssize_t written = write(fd, line, len);

    if(written < 0)
    {
        perror("file write error");

        close(fd);

        return -1;
    }

    if(close(fd) < 0)
    {
        perror("error closing file");

        return -1;
    }

    return 0;
}

int customRandomizer()
{
    srand(time(NULL));

    return rand() % 2;
}


void addCarHandler()
{
    bool flag = false, flagVin = false, flagModel = false, flagManufacturer = false;

    char vin[MAX_LENGTH], model[MAX_LENGTH], manufacturer[MAX_LENGTH];

    do
    {
        clearScreen();

        addCarMenu();
        
        do
        {
            int rc = getLine("Enter the vin: ", vin, MAX_LENGTH - 1);

            if(rc == NO_INPUT)
            {
                // we got no input
                flagVin = false;
            }

            if(rc == TOO_LONG)
            {
                // input too long
                flagVin = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagVin = true;
            }

        } while (!flagVin);

        do
        {
            int rc = getLine("Enter the model: ", model, MAX_LENGTH - 1);

            if(rc == NO_INPUT)
            {
                // we got no input
                flagModel = false;
            }

            if(rc == TOO_LONG)
            {
                // input too long
                flagModel = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagModel = true;
            }
            
        } while (!flagModel);

        do
        {
            int rc = getLine("Enter the manufacturer: ", manufacturer, MAX_LENGTH - 1);

            if(rc == NO_INPUT)
            {
                // we got no input
                flagManufacturer = false;
            }

            if(rc == TOO_LONG)
            {
                // input too long
                flagManufacturer = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagManufacturer = true;
            }
            
        } while (!flagManufacturer);

        printf("Vin: %s, Manufacturer: %s, model: %s\n", vin, manufacturer, model);

        flag = true;

        mySleep(1);
        
    } while (!flag);
    
    char filepath[] = "../resources/rental.csv";

    char repairPath[] = "../resources/rapairShop.csv";

    if(addCarToCSV(filepath, vin, model, manufacturer) == 0)
    {
        printf("Car added to fleet\n");
    }
    else
    {
        printf("Car breakdown!\n");
    }

    char **statusCar = (char **)malloc(2 * sizeof(char *));

    if (statusCar == NULL) 
    {
        perror("Failed to allocate memory for statusCar");

        return;
    }

    statusCar[0] = (char *) malloc((strlen("new") + 1) * sizeof(char));

    if (statusCar[0] == NULL) 
    {
        perror("Failed to allocate memory for statusCar[0]");

        free(statusCar); 
        
        return;
    }

    strcpy(statusCar[0], "new");

    statusCar[1] = (char *)malloc((strlen("damaged") + 1) * sizeof(char));
    
    if (statusCar[1] == NULL) 
    {
        perror("Failed to allocate memory for statusCar[1]");

        free(statusCar[0]); 
        
        free(statusCar);
        
        return;
    }
    
    strcpy(statusCar[1], "damaged");

    char *temp = statusCar[customRandomizer()];

    // printf("status: %s\n", temp);

    // printf("car status to file: %d\n", addToRepairShopCSV(repairPath, vin, temp));

    if(addToRepairShopCSV(repairPath, vin, temp) == 0)
    {
        printf("Car added to repairshop list\n");
    }
    else
    {
        printf("no service programation for you!\n");
    }
     
    // Free allocated memory
    free(statusCar[0]);

    free(statusCar[1]);
    
    free(statusCar);
}

void repairCarHandler()
{
    char fileName[] = "../resources/rapairShop.csv";

    char *line = NULL;

    char **vinArray = NULL;
    
    int numVins = 0;

    FILE* fd = fopen(fileName, "r");

    if (!fd) 
    {
        perror("error opening file");

        return;
    }

    size_t len = 0;
    
    int read;

    int linesRead = 0;

    while ((read = getline(&line, &len, fd)) != -1) 
    {
        removeNewline(line);

        printf("Line read: %s\n", line);

        if (linesRead == 0) 
        {
            ++ linesRead;

            continue;
        }

        char *data = (char *) malloc(read + 1);

        if (!data) 
        {
            perror("Error allocating memory");

            fclose(fd);
            
            free(line);
        }

        strncpy(data, line, read);

        printf("here we have the data: %s\n", data);    

        data[read] = '\0';

        char* token = strtok(data, ",");

        char **temp = (char **) realloc(vinArray, (numVins + 1) * sizeof(char *));

        if(!temp)
        {
            perror("memory failure");

            fclose(fd);

            for(int j = 0; j < numVins; ++ j)
            {
                free(vinArray[j]);
            }

            free(vinArray);

            return;
        }

        vinArray = temp;

        vinArray[numVins] = (char *) malloc(strlen(token) + 1);

        if(!vinArray[numVins])
        {
            perror("malloc failed");

            fclose(fd);

            for(int j = 0; j < numVins; ++ j)
            {
                free(vinArray[j]);
            }

            free(vinArray);
        }

        strcpy(vinArray[numVins], token);

        ++numVins;

        ++linesRead;
    }

    bool flag = false, flagVin = false;

    char vin[MAX_LENGTH];

    do
    { 
        clearScreen();
        
        repairCarMenu();
        
        do
        {
            int rc = getLine("Enter the vin: ", vin, MAX_LENGTH);

            if(rc == NO_INPUT)
            {
                //we got no input
                flagVin = false;
            }

            if(rc == TOO_LONG)
            {
                //input too long
                flagVin = false;
            }

            if(rc == OK || rc == SMALL_BUFF)
            {
                flagVin = true;
            }

        } while (!flagVin);

        printf("VIN: %s\n", vin);

        flag = true;

        mySleep(1);

    } while (!flag);

    bool flagFound = false;

    pthread_mutex_t found_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_t threads[NO_THREADS];

    ThreadData threadData[NO_THREADS];

    int workloadPerThread = numVins / NO_THREADS;

    for(int i = 0; i < NO_THREADS; ++ i)
    {
        threadData[i].vinArray = vinArray;

        threadData[i].start = i * workloadPerThread;

        if(i == NO_THREADS - 1)
        {
            threadData[i].end = numVins;
        }
        else
        {
            threadData[i].end = (i + 1) * workloadPerThread;
        }

        threadData[i].vin = vin;

        threadData[i].found = &flagFound;

        threadData[i].found_mutex = &found_mutex;

        pthread_create(&threads[i], NULL, threadedSearchVIN, &threadData[i]);
    }

    for(int i = 0; i < NO_THREADS; ++ i)
    {
        pthread_join(threads[i], NULL);
    }

    if(!flagFound)
    {
        printf("No car was fond!\n");
    }
    else
    {
        printf("Car was fixed\n");
    }

    // Clean this garbage
    fclose(fd);

    for (int i = 0; i < numVins; ++ i) 
    {
        free(vinArray[i]);
    }

    free(vinArray);
}