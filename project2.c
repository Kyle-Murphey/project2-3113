#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "storage_remote.h"

#define MAX_BUF 128
#define MAX_ARGS 4
//#define STORAGE_NAME "storage.bin"

/*
 *  Project 2
 * Kyle Murphey
 * 10/17/19
 */

/* separators for tokens/inputs */
const char SEPARATORS[] = " \t\n";

/* renaming unsigned char to byte */
typedef unsigned char byte;

/*
 * stores integer value into the buffer
 * @param:args = array of arguments passed in by the cl
 * @param:buffer = buffer in use created in main
 */
void input_int(byte * args[], byte * buffer)
{
    byte * checkArgs = args[2]; //used to scan over inputs to check for invalid commands/values

    //checks if value was inputted
    if (args[2] == NULL)
    {
        fprintf(stderr, "no value given\n");
        return;
    }

    //checks each digit for int value
    for (int i = 0; i < strlen(args[2]); ++i)
    {
        if (*checkArgs != '-' && (*checkArgs < '0' || *checkArgs > '9'))
        {
            fprintf(stderr, "invalid integer\n");
            return;
        }
        ++checkArgs;
    }
    int value = atoi(args[2]); //value to store
    int location = atoi(args[1]); //location in buffer to store
    int * ptr_location = (int*)(&buffer[location]); //pointer to the location in the buffer to store value
    *ptr_location = value; //set val of location to inputted value
}

/*
 * stores byte value into the buffer
 * @param:args = array of arguments passed in by the cl
 * @param:buffer = buffer in use created in main
 */
void input_byte(byte * args[], byte * buffer)
{
    //checks if value was inputted
    if (args[2] == NULL)
    {
        fprintf(stderr, "no value given\n");
        return;
    }

    byte value = atoi(args[2]); //value to store
    int location = atoi(args[1]); //location in buffer to store
    byte * ptr_location = (byte*)(&buffer[location]); //pointer to the location in the buffer to store the value
    *ptr_location = value; //set val of location to inputted value
}

/*
 * stores hex value into the buffer
 * @param:args = array of arguments passed in by the cl
 * @param:buffer = buffer in use created in main
 */
void input_hex(byte * args[], byte * buffer)
{
    byte * checkArgs = args[2]; //used to scan over inputs to check for invalid commands/values

    //checks if value was inputted
    if (args[2] == NULL)
    {
        fprintf(stderr, "no value given\n");
        return;
    }

    //checks each digit for hex values
    for (int i = 0; i < strlen(args[2]); ++i)
    {
        //checks if each character is a number or letter
        if (!(*checkArgs >= '0' && *checkArgs <= '9') && !(*checkArgs >= 'a' && *checkArgs <= 'z') && !(*checkArgs >= 'A' && *checkArgs <= 'Z'))
        {
            fprintf(stderr, "invalid hex\n");
            return;
        }
        ++checkArgs;
    }
    int hex = (int)strtol(args[2], NULL, 16); //value to store; converts input to long and then int
    int location = atoi(args[1]); //location in buffer to store
    byte * ptr_location = (byte*)(&buffer[location]); //pointer to the location in the buffer to store the value
    *ptr_location = hex; //set val of location to inputted value
}

/*
 * stores character value into the buffer
 * @param:args = array of arguments passed in by the cl
 * @param:buffer = buffer in use created in main
 */
void input_char(byte * args[], byte * buffer)
{
    //checks if value was inputted
    if (args[2] == NULL)
    {
        fprintf(stderr, "no value given\n");
        return;
    }

    char value = *args[2]; //value to store
    int location =  atoi(args[1]); //location in buffer to store
    char * ptr_location = (char*)(&buffer[location]); //pointer to the location in the buffer to store the value
    *ptr_location = value; //set val of location to inputted value
}

/*
 * stores float value into the buffer
 * @param:args = array of arguments passed in by the cl
 * @param:buffer = buffer in use created in main
 */
void input_float(byte * args[], byte * buffer)
{
    byte * checkArgs = args[2]; //used to scan over inputs to check for invalid commands/values

    //checks if value was inputted
    if (args[2] == NULL)
    {
        fprintf(stderr, "no value given\n");
        return;
    }

    //checks each digit for float value
    for (int i = 0; i < strlen(args[2]); ++i)
    {
        //checks if each value is a number, -, or .
        if ((*checkArgs != '-' && *checkArgs != '.') && (*checkArgs < '0' || *checkArgs > '9'))
        {
            fprintf(stderr, "invalid float\n");
            return;
        }
        ++checkArgs;
    }
    float value = atof(args[2]); //value to store
    int location = atoi(args[1]); //location in buffer to store
    float * ptr_location = (float*)(&buffer[location]); //pointer to the location in the buffer to store the value
    *ptr_location = value; //set val of location to inputted value
}

/*
 * stores string value into the buffer
 * @param:args = array of arguments passed in by the cl
 * @param:buffer = buffer in use created in main
 */
void input_string(byte * args[], byte * buffer)
{
    //checks if value was inputted
    if (args[2] == NULL)
    {
        fprintf(stderr, "no value given\n");
        return;
    }
    
    char * string = args[2]; //string to store
    int strLength = strlen(args[2]); //length of the string
    int location = atoi(args[1]); //location in buffer to store

    //stores each char of string into buffer
    for (int i = 0; i < strLength; ++i)
    {
        buffer[location++] = *string++;
    }
}

/*
 * creates a buffer, opens/creates a file "storage.bin", stores/reads values from the buffer,
 * reads/writes values from the file to the buffer, checks for errors in commands and then closes file at the end
 * @param:argc = number of arguments
 * @param:argv = arguments inputted (none needed)
 */
int main(int argc, char **argv)
{
    byte buffer[MAX_BUF] = {0}; //buffer to store values in
    byte input[50]; //stores input from cl
    byte * args[MAX_ARGS] = {0}; //stores arguments from input
    byte ** arg; //pointer to arguments
    byte * checkArgs; //used to check for invalid commands
    char * STORAGE_NAME = "storage.bin"; //default storage name

    //check for too many args
    if (argc > 2)
    {
        fprintf(stderr, "invalid arguments");
        exit(-1);
    }
    //user specified storage
    else if (argc == 2)
    {
        STORAGE_NAME = argv[1];
    }

    STORAGE * file = init_storage(STORAGE_NAME); //open or create file for storage

    //main input loop, continues until EOF
    while (fgets(input, 50, stdin))
    {
        //tokenizes input and stores in args array
        arg = args;
        *arg++ = strtok(input, SEPARATORS);
        while((*arg++ = strtok(NULL, SEPARATORS)));

        //only enter is pressed
        if (args[0] == NULL) continue;

        //checks if first input is only one character
        if (strlen(args[0]) != 1)
        {
            fprintf(stderr, "invalid command\n");
            continue;
        }
        
        //zero out the buffer
        if (*args[0] == 'z')
        {
            memset(buffer, 0, sizeof(buffer));
            continue;
        }
        //list contents of buffer
        else if (*args[0] == 'l')
        {
            for (int i = 0; i < 128; ++i)
            {
                printf("%02x ", buffer[i]);
                if ((i + 1) % 16 == 0)
                {
                    printf("\n");
                }
            }
            continue;
        }

        //makes sure more than one param was entered
        if (args[1] != NULL)
        {
            checkArgs = args[1]; //2nd parameter
            int isError = 0; //flag for bad location
            
            //checks each digit for location
            for (int i = 0; i < strlen(args[1]); ++i)
            {
                if (*checkArgs < '0' || *checkArgs > '9' )
                {
                    fprintf(stderr, "invalid location\n");
                    isError = 1;
                    break;
                }
                ++checkArgs;
            }
            //restart main loop
            if (isError) continue;
        }
        //no second argument
        else
        {
            fprintf(stderr, "not enough parameters\n");
            continue;
        }
        
        //writes content from the buffer to the file
        if (*args[0] == 'w')
        {
            int write = put_bytes(file, buffer, atoi(args[1]), atoi(args[2]));
        }
        //reads content from the file into the buffer
        else if (*args[0] == 'r')
        {
            int read = get_bytes(file, buffer, atoi(args[1]), atoi(args[2]));
        }
        //input integer value
        else if (*args[0] == 'i')
        {
            input_int(args, buffer);
        }
        //prints integer output
        else if (*args[0] == 'I')
        {
            int * ptr_loc = (int*)(&buffer[atoi(args[1])]); //pointer to location in buffer to read
            printf("%d\n", *ptr_loc); //print integer at given location
        }
        //input byte value
        else if (*args[0] == 'b')
        {
            input_byte(args, buffer);
        }
        //prints byte value
        else if (*args[0] == 'B')
        {
            byte * ptr_loc = (byte*)(&buffer[atoi(args[1])]); //pointer to location in buffer to read
            printf("%d\n", *ptr_loc); //print byte at given location
        }
        //input hex value
        else if (*args[0] == 'h')
        {
            input_hex(args, buffer);
        }
        //prints hex value
        else if (*args[0] == 'H')
        {
            byte * ptr_loc = (byte*)(&buffer[atoi(args[1])]); //pointer to location in buffer to read
            printf("%x\n", *ptr_loc); //print hex at given location
        }
        //input char value
        else if (*args[0] == 'c')
        {
            input_char(args, buffer);
        }
        //prints char value
        else if (*args[0] == 'C')
        {
            char * ptr_loc = (char*)(&buffer[atoi(args[1])]); //pointer to location in buffer to read
            printf("%c\n", *ptr_loc); //print character at given location
        }
        //input float value
        else if (*args[0] == 'f')
        {
            input_float(args, buffer);
        }
        //prints float value
        else if (*args[0] == 'F')
        {
            float * ptr_loc = (float*)(&buffer[atoi(args[1])]); //pointer to location in buffer to read
            printf("%f\n", *ptr_loc); //print float at given location
        }
        //input string value
        else if (*args[0] == 's')
        {
            input_string(args, buffer);
        }
        //prints string value
        else if (*args[0] == 'S')
        {
            char * ptr_loc = (char*)(&buffer[atoi(args[1])]); //pointer to location in buffer to read
            printf("%s\n", ptr_loc); //print string at given location
        }
        //bad command
        else
        {
            fprintf(stderr, "invalid command\n");
        }
    }

    //closing file
    int close = close_storage(file);
    free(file);
}