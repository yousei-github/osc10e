/**
 * multi-fork.c
 *
 * How many processes are created??
 */

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
