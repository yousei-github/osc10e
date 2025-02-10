/**
 * @file
 * fig3-32.c
 * 
 * @details
 * Including the initial parent process, how many processes are created by the program shown in Figure 3.32?
 * 
 * Figure 3.32
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

    int i = 0;

    for (; i < 4; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            /* Child process */
            printf("A new child process.\n");
        }
    }

    /* Total number of processes is 2^4 = 16 */

    return EXIT_SUCCESS;
}
