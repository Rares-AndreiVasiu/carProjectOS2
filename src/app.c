#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../inc/app.h"
#include "../inc/utility.h"

#include "../inc/menu.h"

void runApp()
{
    int option = 1;

    do
    {
        entraceMenu();

        #if DBG == 1
            printf("Hello there stranger!\n");
        #endif

        //first we have the entrace menu
        option = getUserOption(LOW_BOUND, UPPER_BOUND_ENTRACE);

        switch (option)
        {
            case 1:
            {
                // printf("Here we login!\n");

                loginHandler();

                break;
            }

            case 2:
            {
                // create account
                printf("Here we create an account!\n");

                createAccountHandler();
                
                break;
            }

            case 3:
            {
                //rent a car
                printf("Rent a car!\n");

                rentalHandler();

                break;
            }

            case 4:
            {
                //add car

                printf("we add a new rental car\n");

                addCarHandler();

                break;
            }

            case 5:
            {
                //repair defected car

                printf("we repair a poor car\n");

                repairCarHandler();

                break;
            }

            case 6:
            {
                printf("Here we exit the app!\n");

                mySleep(2);

                exit(EXIT_SUCCESS);
            }

            default:
            {
                clearScreen();

                break;
            }
        }

    } while (true);
}
