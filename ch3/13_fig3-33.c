/**
 * @file
 * fig3-33.c
 * 
 * @details
 * Explain the circumstances under which the line of code marked printf("LINE J") in Figure 3.33 will be reached.
 * 
 * Figure 3.33
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
        execlp("/bin/ls", "ls", NULL);
        printf("LINE J"); // This line won't be executed since the memory of child process is replaced by ls command
    }
    else
    {
        /* Parent process */

        /* Parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete\n");
    }

    return EXIT_SUCCESS;
}
