/**
 * @file
 * unix_pipe.c
 * 
 * @brief
 * UNIX Ordinary Pipes
 * 
 * @details
 * Example program demonstrating UNIX ordinary pipes.
 * 
 * A pipe acts as a conduit allowing two processes to communicate. In implementing a pipe, four issues must be considered:
 * - Does the pipe allow bidirectional communication, or is communication unidirectional?
 * - If two-way communication is allowed, is it half duplex (data can travel only one way at a time) or full duplex (data can travel in both directions at the same time)?
 * - Must a relationship (such as parent–child) exist between the communicating processes?
 * - Can the pipes communicate over a network, or must the communicating processes reside on the same machine?
 * 
 * Ordinary pipes allow two processes to communicate in standard producer–consumer fashion:
 * the producer writes to one end of the pipe (the write end) and the consumer reads from the other end (the read end).
 * As a result, ordinary pipes are unidirectional, allowing only one-way communication.
 * If two-way communication is required, two pipes must be used, with each pipe sending data in a different direction.
 * 
 * UNIX treats a pipe as a special type of file.
 * 
 * An ordinary pipe cannot be accessed from outside the process that created it.
 * Typically, a parent process creates a pipe and uses it to communicate with a child process that it creates via fork().
 * (A child process inherits open files from its parent)
 * 
 * Figures 3.21 & 3.22
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#ifndef __linux__
#error "Must be using Linux kernel!"
#endif /* __linux__ */

/* Header */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/* Macro */

#define BUFFER_SIZE (25)

/* Type */

typedef enum
{
    Read_End  = 0,
    Write_End = 1,
    Pipe_Index_Max
} Pipe_File_Descriptor_Index;

/* Prototype */

/* Variable */

/* Function */

int main(void)
{
    const char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE]        = {};
    pid_t process_ID                  = {0};

    /** 
     * UNIX treats a pipe as a special type of file.
     * Thus, pipes can be accessed using ordinary read() and write() system calls.
     * 
     * Any writes by the parent to its write end of the pipe—fd[1]—can be read by the child from its read end—fd[0]—of the pipe.
     * 
     * @note
     * It is important to notice that both the parent process and the child process initially close their unused ends of the pipe.
     */
    int fd[Pipe_Index_Max];

    /* Create the pipe */
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe failed\n");
        return EXIT_FAILURE;
    }

    /* Now fork a child process */
    process_ID = fork();

    if (process_ID < 0)
    {
        fprintf(stderr, "Fork failed\n");
        return EXIT_FAILURE;
    }
    else if (process_ID == 0)
    {
        /* Child process */

        /** Close the unused end of the pipe */
        close(fd[Write_End]);

        /** Read from the pipe */
        read(fd[Read_End], read_msg, BUFFER_SIZE);
        printf("Child reads: %s\n", read_msg);

        /** Close the write end of the pipe */
        close(fd[Read_End]);
    }
    else
    {
        /* Parent process */

        /** Close the unused end of the pipe */
        close(fd[Read_End]);

        /** Write to the pipe */
        printf("Parent writes: %s\n", write_msg);
        write(fd[Write_End], write_msg, strlen(write_msg) + 1);

        /** Close the write end of the pipe */
        close(fd[Write_End]);
    }

    return EXIT_SUCCESS;
}
