/**
 * @file
 * fig3-31.c
 * 
 * @details
 * Including the initial parent process, how many processes are created by the program shown in Figure 3.31?
 * 
 * Figure 3.31
 */

#ifndef __linux__
#error "Must be using Linux kernel!"
#endif /* __linux__ */

/* Header */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* Macro */

/* Type */

/* Prototype */

/* Variable */

/* Function */

int main()
{
    pid_t pid = getpid();
    printf("I am the parent process (PID = %d).\n", pid);

    pid = fork(); // Total number of processes is 2 (1+1)
    if (pid == 0)
    {
        /* Child process */
        printf("A new child process.\n");
    }

    pid = fork(); // Total number of processes is 4 (2+2)
    if (pid == 0)
    {
        /* Child process */
        printf("A new child process.\n");
    }

    pid = fork(); // Total number of processes is 8 (4+4)
    if (pid == 0)
    {
        /* Child process */
        printf("A new child process.\n");
    }

    return EXIT_SUCCESS;
}
