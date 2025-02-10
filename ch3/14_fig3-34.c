/**
 * @file
 * fig3-34.c
 * 
 * @details
 * Using the program in Figure 3.34, identify the values of pid at lines A, B, C, and D.
 * (Assume that the actual pids of the parent and child are 2600 and 2603, respectively.)
 * 
 * Figure 3.34
 */

#ifndef __linux__
#error "Must be using Linux kernel!"
#endif /* __linux__ */

/* Header */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Macro */

/* Type */

/* Prototype */

/* Variable */

/* Function */

int main()
{
    pid_t current_pid;

    /* Fork a child process */
    const pid_t child_pid = fork();

    if (child_pid < 0)
    {
        /* Error occurred */
        fprintf(stderr, "Fork Failed");
        return EXIT_FAILURE;
    }
    else if (child_pid == 0)
    {
        /* Child process */
        current_pid = getpid();
        printf("child: child_pid = %d\n", child_pid);     /* A */
        printf("child: current_pid = %d\n", current_pid); /* B */
    }
    else
    {
        /* Parent process */
        current_pid = getpid();
        printf("parent: child_pid = %d\n", child_pid);     /* C */
        printf("parent: current_pid = %d\n", current_pid); /* D */
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
