/**
 * @file
 * newproc-win32.c
 * 
 * @details
 * This program creates a separate process using the CreateProcess() system call.
 *
 * Figure 3.10
 *
 * @author Gagne, Galvin, Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#ifndef _WIN32
#error "Must be using Windows (32-bit or 64-bit)!"
#endif /* _WIN32 */

/* Header */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/* Macro */

/* Type */

/* Prototype */

/* Variable */

/* Function */

int main(VOID)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process.
    if (! CreateProcess(NULL,                     // No module name (use command line).
            "C:\\WINDOWS\\system32\\mspaint.exe", // Command line.
            NULL,                                 // Process handle not inheritable.
            NULL,                                 // Thread handle not inheritable.
            FALSE,                                // Set handle inheritance to FALSE.
            0,                                    // No creation flags.
            NULL,                                 // Use parent's environment block.
            NULL,                                 // Use parent's starting directory.
            &si,                                  // Pointer to STARTUPINFO structure.
            &pi                                   // Pointer to PROCESS_INFORMATION structure.
            ))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return EXIT_FAILURE;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return EXIT_SUCCESS;
}
