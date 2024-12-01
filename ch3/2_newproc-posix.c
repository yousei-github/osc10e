/**
 * This program forks a separate process using the fork()/exec() system calls.
 *
 * Figure 3.08
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

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
        pid = getpid();
        printf("I am the child %d process\n", pid);
        execlp("/bin/ls", "ls", NULL);
    }
    else
    {
        /* Parent process */
        const pid_t child_pid = pid;
        pid                   = getpid();
        /* Parent will wait for the child to complete */
        printf("I am the parent %d process, my child process is %d\n", pid, child_pid);
        wait(NULL);

        printf("Child Complete\n");
    }

    return EXIT_SUCCESS;
}
