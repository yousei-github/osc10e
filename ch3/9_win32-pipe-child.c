/**
 * @brief Windows Ordinary Pipes
 * 
 * @details
 * Simple program demonstrating Win32 anonymous pipes.
 *
 * Figure 3.25
 *
 * @author Silberschatz, Gagne, and Galvin.
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

/* Header */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/* Macro */

#define BUFFER_SIZE (25)

/* Type */

/* Prototype */

/* Variable */

/* Function */

int main(VOID)
{
    HANDLE ReadHandle, WriteHandle;
    CHAR buffer[BUFFER_SIZE];
    DWORD read;

    /* Obtain the read handle to the pipe */
    ReadHandle  = GetStdHandle(STD_INPUT_HANDLE);
    WriteHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    /* Have the child read from the pipe */
    if (ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL))
        printf("Child reads: %s\n", buffer);
    else
        fprintf(stderr, "Child: Error reading from pipe\n");

    return EXIT_SUCCESS;
}
