/*
* FILE : hex_dump.c
* PROGRAMMER : Marcin Czajkowski
* FIRST VERSION : 2015-11-30
* DESCRIPTION :
*	This program takes user input from command line.
*	The input is a filename to be open as binary
*	and transfered to another (contents.txt).
*	The new file is formatted to represent data as
*	integers in 10 values per line.
*******************************************************
*	WARNING! This program will work only with files
*			 which are up to 2147483647 bytes long.
*******************************************************
*/

// Including headers pointing to necessary libraries and function prototypes
#include "common.h" 
#include "proto.h"

// Preprocessor definitions
#define DEST_FILE "contents.txt"	//destination file
#define MAIN_RETURN_OK 0			//used for "ALL OK" return from main
#define MAIN_RETURN_ERROR 1			//used return from main when an issue occured
#define FINAL_TERMINATOR "\n"		//final termination written to the destination file

#pragma warning(disable: 4996)// CRT_SECURE_NO_WARNINGS

// Start of main
int main(int argc, char* argv[])
{
	int mainReturn = 0; //used to assign main return value
	int fileSize = 0;	//used to get a value of a size of a file in bytes

	FILE* rbFPsource = NULL; //file pointer to source file
	FILE* aFPdestination = NULL; //file pointer to destination file

	const char* kSOURCE_FILE = argv[1]; //the source file sahll be assigned as a const char*

	// The statement below checks if user provided a source file name (more than one command line argument)
	if (argc < 2)
	{
		printf("\nError, no file specified in command line. Exitting program.\n");
		mainReturn = MAIN_RETURN_ERROR;
	}
	else if (argc >= 2)
	{
		if (argc > 2)
		{
			printf("\nMore command line arguments then necassary were provided,\n using first argument as filename\n");
		}

		/*******************************************************
		*	WARNING!This program will work only with files
		*			 which are up to 2147483647 bytes long.
		*******************************************************/
		fileSize = getSmallFileLenght(kSOURCE_FILE); //acquiring source file size

		//If issue occured the getSmallFileLenght() will return -1, statement below checks for that condition
		if (fileSize == (-1))
		{
			mainReturn = MAIN_RETURN_ERROR;
		}
		else
		{
			// Displaying information about the source file (for debbugging purposes only)
			printf("\nThe input filename is: %s\nThe file size is: %d\n", kSOURCE_FILE, fileSize);

			rbFPsource = fopen(kSOURCE_FILE, "rb"); //open source file for reading as binary
			aFPdestination = fopen(DEST_FILE, "a"); //open  destination file for writing as text

			// Statement below checks for files opening errrors
			if (rbFPsource == NULL)
			{
				printf("Error opening the %s file. Exitting program.\n", kSOURCE_FILE);
				mainReturn = MAIN_RETURN_ERROR;
			}
			else if (aFPdestination == NULL)
			{
				printf("Error opening the %s file. Exitting program.\n", DEST_FILE);
				mainReturn = MAIN_RETURN_ERROR;
			}
			else // no errors - the program continues
			{
				char* buffer = malloc(fileSize * sizeof(char)); //allocate space to transfer data from file to buffer
				// Statement below checks if there was an error allocating space for the data
				if (buffer == NULL) {
					printf("\nMalloc fail. Exitting program.\n");
					mainReturn = MAIN_RETURN_ERROR;
				}
				else
				{
					int countChars = 0; //used for countinf ammount of chars before assigning new line 
					fread(buffer, sizeof(char), fileSize, rbFPsource); //pulling all data from a file and assigning it to the allocated buffer/space

					// The loop below goes through the memory space where file was copied and writes it to new file
					for (int loopCount = 0; loopCount < fileSize; ++loopCount)
					{
						int insertChar = buffer[loopCount];
						countChars++;
						// Since the read data is unsigned it requires formatting of negative (overflow) numbers,
						// statement below corrects that issue
						if (insertChar < 0)
						{
							insertChar += 256;
						}

						printf("%03d ", insertChar); //print to screen (for debug purposes)
						fprintf(aFPdestination, "%03d ", insertChar); //insert data to destination file

						// Statement below parses the data to 10 chars per line
						if (countChars == 10)
						{
							countChars = 0;
							printf("\n");
							fprintf(aFPdestination, "\n");
						}
					}

					printf("\n");
					fprintf(aFPdestination, FINAL_TERMINATOR); //final char written to the file - termination

					// Cleanup
					fclose(rbFPsource);
					fclose(aFPdestination);

					mainReturn = MAIN_RETURN_OK;
				}
			}
		}
	}
	return mainReturn;
} //end of main
