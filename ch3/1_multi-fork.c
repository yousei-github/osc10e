/**
 * @file
 * multi-fork.c
 *
 * @note
 * How many processes are created??
 */

#ifndef __linux__
#error "Must be using Linux kernel!"
#endif /* __linux__ */

/* Header */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* Macro */

/* Type */

/* Prototype */

/* Variable */

/* Function */

/** Determine the @p forked_process_ID is parent process ID or not */
bool is_parent_or_child_process(const uint32_t forked_process_ID)
{
    return (forked_process_ID == 0) ? false : true;
}

int main()
{
    pid_t process_ID = getpid();

    printf("PID (Process Identifier) = %d. fork() level: *\n", process_ID);

    pid_t forked_process_ID = fork();
    process_ID              = getpid();
    printf("PID (Process Identifier) = %d. fork() level: ** (Forked PID = %d, is parent process? %d)\n", process_ID, forked_process_ID, is_parent_or_child_process(forked_process_ID));

    forked_process_ID = fork();
    process_ID        = getpid();
    printf("PID (Process Identifier) = %d. fork() level: *** (Forked PID = %d, is parent process? %d)\n", process_ID, forked_process_ID, is_parent_or_child_process(forked_process_ID));

    forked_process_ID = fork();
    process_ID        = getpid();
    printf("PID (Process Identifier) = %d. fork() level: **** (Forked PID = %d, is parent process? %d)\n", process_ID, forked_process_ID, is_parent_or_child_process(forked_process_ID));

    return EXIT_SUCCESS;
}
