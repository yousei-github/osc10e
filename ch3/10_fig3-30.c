/**
 * @file
 * fig3-30.c
 * 
 * @details
 * Using the program shown in Figure 3.30, explain what the output will be at LINE A.
 * 
 * Figure 3.30
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

int value = 5;

/* Function */

int main()
{
    pid_t pid = getpid();
    printf("PID (Process Identifier) = %d.\n", pid);

    /* Fork a child process */
    pid = fork();

    if (pid < 0)
    {
        /* Error occurred */
        fprintf(stderr, "Fork Failed\n");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        /* Child process */
        value += 15;
        printf("Child process: value = %d\n", value);
        return EXIT_SUCCESS;
    }
    else
    {
        /* Parent process */

        /* Parent will wait for the child to complete */
        wait(NULL);

        printf("Parent process: value = %d\n", value); /* LINE A */
        return EXIT_SUCCESS;
    }
}
