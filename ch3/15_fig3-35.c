/**
 * @file
 * fig3-35.c
 * 
 * @details
 * Using the program shown in Figure 3.35, explain what the output will be at lines X and Y.
 * 
 * Figure 3.35
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

#define SIZE 5

/* Type */

/* Prototype */

/* Variable */

int nums[SIZE] = {0, 1, 2, 3, 4};

/* Function */

int main()
{
    int i;

    /* Fork a child process */
    const pid_t pid = fork();

    if (pid < 0)
    {
        /* Error occurred */
        fprintf(stderr, "Fork Failed");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        /* Child process */

        for (i = 0; i < SIZE; i++)
        {
            nums[i] *= -i;
            printf("Child process: %d\n", nums[i]); /* LINE X */
        }
    }
    else
    {
        /* Parent process */

        /* Parent will wait for the child to complete */
        wait(NULL);

        for (i = 0; i < SIZE; i++)
        {
            printf("Parent process: %d\n", nums[i]); /* LINE Y */
        }
    }

    return EXIT_SUCCESS;
}
