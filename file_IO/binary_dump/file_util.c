/*
* FILE : file_util.c
* PROGRAMMER : Marcin Czajkowski
* FIRST VERSION : 2015-11-30
* DESCRIPTION :
*	This code checks if a file exists and if it is a file or a directory
*   if it is a file it return its size in bytes.
*******************************************************
*	WARNING! This program will work only with files
*			 which are up to 2147483647 bytes long.
*******************************************************
*/

// This header includes all basic libraries to run this code
#include "common.h"

// FUNCTION: getSmallFileLenght()
// DESCRIPTION: This function acquires a file size
// PARAMETERS: const char* filename - name of the file to get a size of
// RETURNS: int - size of a file in bytes
int getSmallFileLenght(const char* filename)
{
	int functionReturn = 0;

	HANDLE hFind = INVALID_HANDLE_VALUE; //handle assignment
	WIN32_FIND_DATA FindFileData = { 0 }; //declaration of new struct

	hFind = FindFirstFile(filename, &FindFileData); //looking for a file and its specifications

	// The statement below confirms that the file exists and it is not a directory
	if ((hFind == INVALID_HANDLE_VALUE) ||
		((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0))
	{
		printf("\nError. Incorrect file entered as argument\n"); // print error if conditions not met
		functionReturn = -1;
	}
	else
	{
		functionReturn = FindFileData.nFileSizeLow; //acquire file size
	}

	// Cleanup
	FindClose(hFind);

	return functionReturn;
} // end of function
