#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/****************
-This is CMPE 351 Project program
-This program simulates the CPU Scheduler methods
*/

/*TODO
-Need to implement a way to check if the passed parameters are a txt file
-Need to implement a linked list structure and funtions
-Need to implement a queue structure and funtions
-Need to implement a custom structure for reading the input file
*/

//This funtions is used to print programs usage and what arguments are needed to pass
void print_usage(){
        printf("Usage: cmpe351 -f <*.txt> -o <*.txt>\n");
        exit(2);
}

int main(int argc, char *argv[]){
        int options = 0;
        char* input_filename = NULL;
        char* output_filename = NULL;

        //Here we check if the parameters are passed
        while((options = getopt(argc, argv, "f:o:")) != -1)
        {
                switch(options)
                {
                        case 'f':
                                input_filename = optarg;
                                break;
                        case 'o':
                                output_filename = optarg;
                                break;
                        default:
                                print_usage();
                                break;
                }
        }


        return 0;
}
