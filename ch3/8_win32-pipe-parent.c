/**
 * @brief Windows Ordinary Pipes
 * 
 * @details
 * Simple program demonstrating Win32 anonymous pipes.
 * 
 * Ordinary pipes on Windows systems are termed anonymous pipes, and they behave similarly to their UNIX counterparts:
 * they are unidirectional and employ parent–child relationships between the communicating processes.
 *
 * Figures 3.23 & 3.24
 *
 * @author Silberschatz, Gagne and Galvin.
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

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    const char message[BUFFER_SIZE] = "Greetings";
    DWORD written;

    /** 
  * Set up security attributes (SECURITY_ATTRIBUTES structure) so that pipe handles are inherited,
  * and then redirecting the child process's handles for standard input or standard output to the read or write handle of the pipe.
 */
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

    /* Create the pipe */
    if (! CreatePipe(&ReadHandle, &WriteHandle, &sa, 0))
    {
        fprintf(stderr, "Create Pipe Failed\n");
        return EXIT_FAILURE;
    }

    /* Establish the START_INFO structure for the child process */
    GetStartupInfo(&si);
    si.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    /* Redirect the standard input to the read end of the pipe */
    si.hStdInput  = ReadHandle;
    si.dwFlags    = STARTF_USESTDHANDLES;

    /* Allocate memory */
    ZeroMemory(&pi, sizeof(pi));

    /* We do not want the child to inherit the write end of the pipe */
    SetHandleInformation(WriteHandle, HANDLE_FLAG_INHERIT, 0);

    /* Create the child process */
    if (! CreateProcess(NULL,                   // No module name (use command line).
            ".\\bin\\9_win32-pipe-child.c.exe", // Command line.
            NULL,                               // Process handle not inheritable.
            NULL,                               // Thread handle not inheritable.
            TRUE,                               // The child process is to inherit designated handles from its parent
            0,                                  // No creation flags.
            NULL,                               // Use parent's environment block.
            NULL,                               // Use parent's starting directory.
            &si,                                // Pointer to STARTUPINFO structure.
            &pi                                 // Pointer to PROCESS_INFORMATION structure.
            ))
    {
        fprintf(stderr, "Process Creation Failed\n");
        return EXIT_FAILURE;
    }

    /* Close the unused end of the pipe */
    CloseHandle(ReadHandle);

    /* The parent now wants to write to the pipe */
    printf("Parent writes: %s\n", message);
    if (! WriteFile(WriteHandle, message, BUFFER_SIZE, &written, NULL))
        fprintf(stderr, "Error writing to pipe\n");

    /* Close the write end of the pipe */
    CloseHandle(WriteHandle);

    /* Wait for the child to exit */
    WaitForSingleObject(pi.hProcess, INFINITE);

    /* Close all handles */
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return EXIT_SUCCESS;
}
