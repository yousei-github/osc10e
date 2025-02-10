/**
 * @file
 * simple-shell.c
 * 
 * @details
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#ifndef __linux__
#error "Must be using Linux kernel!"
#endif /* __linux__ */

/* Header */

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

/* Macro */

#define MAX_USER_INPUT_LENGTH (UINT16_MAX) // Unit: Byte

/* Type */

/* Prototype */

void* create_shared_memory_for_communication(const char* shared_memory_name);
void* link_shared_memory_for_communication(const char* shared_memory_name);
void unlink_shared_memory(const char* shared_memory_name);

/* Variable */

const char* const shared_memory_name = "Shell Input";

/* Function */

int main(void)
{
    // char* args[MAX_USER_INPUT_LENGTH / 2 + 1]; /* command line (of 80) has max of 40 arguments */
    char* const user_input = (char*) malloc(MAX_USER_INPUT_LENGTH);
    if (user_input == NULL)
    {
        printf("%s: Memory Allocation Error.", __func__);
        abort();
    }

    bool should_run = true;

    while (should_run)
    {
        printf("osh> ");
        fflush(stdout); // Flushes the output buffer of the stdout stream

        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */

        scanf("%s", user_input); /** @todo How to read multiple parameters at the same line */

        printf("user_input=%s\n", user_input);

        const char* const delimiter = " ";
        char* token                 = NULL;
        /* Get the first token */
        token                       = strtok(user_input, delimiter);

        if (strcmp(token, "exit") == 0)
        {
            should_run = false;
        }
        else
        {
            void* const ptr = create_shared_memory_for_communication(shared_memory_name);
            if (ptr == NULL)
            {
                printf("Create shared memory failed\n");
                return EXIT_FAILURE;
            }

            memcpy(ptr, user_input, MAX_USER_INPUT_LENGTH);

            /* Fork a child process */
            const pid_t pid = fork();

            if (pid < 0)
            {
                /* Error occurred */
                fprintf(stderr, "Fork Failed\n");
                return EXIT_FAILURE;
            }
            else if (pid == 0)
            {
                /* Child process */

                void* const ptr             = link_shared_memory_for_communication(shared_memory_name);

                char file_path[UINT8_MAX]   = "/bin/";

                const char* const delimiter = " ";
                /** Get the command name */
                char* const command_name    = strtok(ptr, delimiter);
                strcat(file_path, command_name);

                /* Get the arguments */
                char* const arguments = strtok(NULL, "");

                printf("arguments=%s\n", arguments);

                if (arguments == NULL)
                {
                    execlp(file_path, command_name, NULL);
                }
                else
                {
                    execlp(file_path, command_name, arguments, NULL);
                }
            }
            else
            {
                /* Parent process */

                /* Parent will wait for the child to complete */
                wait(NULL);

                unlink_shared_memory(shared_memory_name);
            }
        }
    }

    return EXIT_SUCCESS;
}

/**
 * @brief
 * Create a shared memory that can be written and read
 * 
 * @param[in] shared_memory_name The name of shared memory
 * @return The pointer of created shared memory. Return NULL if it failed
 */
void* create_shared_memory_for_communication(const char* shared_memory_name)
{
    const int shm_fd = shm_open(shared_memory_name, O_CREAT | O_RDWR, 0660);
    if (shm_fd == -1)
    {
        printf("Open shared memory segment failed\n");
        return NULL;
    }

    ftruncate(shm_fd, MAX_USER_INPUT_LENGTH);

    void* const ptr = mmap(0, MAX_USER_INPUT_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        return NULL;
    }

    return ptr;
}

/**
 * @brief
 * Link a created shared memory that can be written and read
 * 
 * @param[in] shared_memory_name The name of shared memory
 * @return The pointer of linked shared memory. Return NULL if it failed
 */
void* link_shared_memory_for_communication(const char* shared_memory_name)
{
    const int shm_fd = shm_open(shared_memory_name, O_RDONLY, 0440);
    if (shm_fd == -1)
    {
        printf("Open shared memory failed\n");
        return NULL;
    }

    void* const ptr = mmap(0, MAX_USER_INPUT_LENGTH, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        return NULL;
    }

    return ptr;
}

/**
 * @brief
 * Unlink a shared memory
 * 
 * @param[in] shared_memory_name The name of shared memory
 */
void unlink_shared_memory(const char* shared_memory_name)
{
    if (shm_unlink(shared_memory_name) == -1)
    {
        printf("Error removing %s\n", shared_memory_name);
    }
}
