#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../inc/app.h"
#include "../inc/utility.h"

void runApp()
{
    int option = 1;

    do
    {
        #if DBG == 1
            printf("Hello there stranger!\n");
        #endif

        //first we have the entrace menu
        option = getUserOption(LOW_BOUND, UPPER_BOUND_ENTRACE);

        switch (option)
        {
            case 1:
            {
                printf("Here we login!\n");

                break;
            }

            case 2:
            {
                printf("Here we create an account!\n");
                
                break;
            }

            case 3:
            {
                printf("Here we exit the app!\n");

                mySleep(2);

                exit(EXIT_SUCCESS);
            }

            default:
            {
                break;
            }
        }

    } while (true);
}
