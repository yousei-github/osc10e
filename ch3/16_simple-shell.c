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

#include <assert.h>
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

// Shared memory interface (Shared memory controller)
void* create_shared_memory_for_communication(const char* shared_memory_name);
void* link_shared_memory_for_communication(const char* shared_memory_name);
void unlink_shared_memory(const char* shared_memory_name);

// Argument processing interface (execvp factory)
char* create_file_path(char* const destination, char* const command_name);
uint32_t count_number_of_arguments(const char* arguments);
char** create_input_parameter_list(char* const command_name, char* const arguments);
void free_input_parameter_list(void* address);

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

        // Reads a line from the specified stream
        const char* input_result = fgets(user_input, MAX_USER_INPUT_LENGTH, stdin);
        if (input_result == NULL)
        {
            printf("%s: Input Parameter Error.", __func__);
            continue;
        }

        char* const last_character = &(user_input[strlen(user_input) - 1]);
        // Replace last character with null character
        if (*last_character == '\n')
        {
            *last_character = '\0';
        }
        // printf("user_input=%s\n", user_input);

        const char exit_command[] = "exit"; // Exit command

        if (strncmp(user_input, exit_command, strlen(exit_command)) == 0)
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
            pid_t pid = fork();

            if (pid < 0)
            {
                /* Error occurred */
                fprintf(stderr, "Fork Failed\n");
                return EXIT_FAILURE;
            }
            else if (pid == 0)
            {
                /* Child process */

                void* const ptr = link_shared_memory_for_communication(shared_memory_name);

                memcpy(user_input, ptr, MAX_USER_INPUT_LENGTH);
                // printf("ptr=%s, user_input=%s\n", (char*) ptr, (char*) user_input);

                const char* const delimiter = " ";
                /** Get the command name */
                char* const command_name    = strtok(user_input, delimiter);

                char file_path[UINT8_MAX]   = {};
                create_file_path(file_path, command_name);

                /* Get the rest of arguments */
                char* const arguments = strtok(NULL, "");
                // printf("arguments=%s, user_input=%s\n", arguments, user_input);

                int execution_result  = EXIT_SUCCESS;
                if (arguments == NULL)
                {
                    execution_result = execlp(file_path, command_name, NULL);
                }
                else
                {
                    char** const input_parameter_list = create_input_parameter_list(command_name, arguments);
                    execution_result                  = execvp(file_path, input_parameter_list);
                }

                if (execution_result != EXIT_SUCCESS)
                {
                    /* Error occurred */
                    fprintf(stderr, "Execution Failed: file_path=%s, arguments=%s\n", file_path, arguments);
                    return EXIT_FAILURE;
                }
            }
            else
            {
                /* Parent process */
                const pid_t child_pid = pid;

                /* Parent will wait for a child to complete */
                int child_status      = EXIT_SUCCESS;
                pid                   = wait(&child_status);

                if (pid == child_pid)
                {
                    // printf("Child complete: status=%d\n", child_status);
                }

                unlink_shared_memory(shared_memory_name);
            }
        }
    }

    free(user_input);

    return EXIT_SUCCESS;
}

/** Shared memory controller */

/**
 * @brief
 * Create a shared memory that can be written and read
 * 
 * @param[in] shared_memory_name Name of shared memory
 * @return Pointer of created shared memory. Return NULL if it failed
 */
void* create_shared_memory_for_communication(const char* shared_memory_name)
{
    assert(shared_memory_name != NULL);

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
 * @param[in] shared_memory_name Name of shared memory
 * @return Pointer of linked shared memory. Return NULL if it failed
 */
void* link_shared_memory_for_communication(const char* shared_memory_name)
{
    assert(shared_memory_name != NULL);

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
 * @brief Unlink a shared memory
 * 
 * @param[in] shared_memory_name Name of shared memory
 */
void unlink_shared_memory(const char* shared_memory_name)
{
    assert(shared_memory_name != NULL);

    if (shm_unlink(shared_memory_name) == -1)
    {
        printf("Error removing %s\n", shared_memory_name);
    }
}

/** Execvp factory */

/**
 * @brief Create the file path for execlp(), execvp()
 * 
 * @param[in] destination  String to store file path
 * @param[in] command_name Command Name
 * 
 * @return File path
 */
char* create_file_path(char* const destination, char* const command_name)
{
    assert((destination != NULL) && (command_name != NULL));

    const char path_separator = '/';

    // Does the command name has absolute path (/) or relative path (./) or just itself?
    if (command_name[0] == path_separator)
    {
        // For absolute path, nothing to do here
    }
    else if (strncmp(command_name, "./", 2) == 0)
    {
        // For relative path, nothing to do here
    }
    else
    {
        // For command name itself, load the default executable file directory
        strcpy(destination, "/bin/");
    }
    strcat(destination, command_name);

    return destination;
}

/**
 * @brief Count the number of arguments
 * 
 * @param[in] arguments Arguments array
 * @return Number of arguments
 */
uint32_t count_number_of_arguments(const char* arguments)
{
    assert(arguments != NULL);

    uint32_t number_of_arguments    = 0;
    const uint32_t arguments_length = strlen(arguments) + 1; // Byte
    char* const arguments_copy      = (char*) malloc(arguments_length);
    if (arguments_copy == NULL)
    {
        printf("%s: Memory Allocation Error.", __func__);
        abort();
    }

    memcpy(arguments_copy, arguments, arguments_length);

    const char* const delimiter = " ";
    char* token                 = strtok(arguments_copy, delimiter);
    while (token != NULL)
    {
        // printf("token=%s\n", token);
        number_of_arguments++;
        token = strtok(NULL, delimiter);
    }

    free(arguments_copy);

    return number_of_arguments;
}

/**
 * @brief Create the input parameter list for execvp()
 * 
 * @param[in] command_name Name of command to execute
 * @param[in] arguments    Arguments array for command_name
 * @return Pointer of input parameter list
 */
char** create_input_parameter_list(char* const command_name, char* const arguments)
{
    assert((command_name != NULL) && (arguments != NULL));

    uint32_t number_of_arguments    = count_number_of_arguments(arguments);

    // Prepare input parameters for execvp()
    const uint8_t char_pointer_size = sizeof(char*);
    number_of_arguments += 2; // First parameter stores command name and last parameter stores NULL
    char** const input_parameter_list = (char**) malloc(number_of_arguments * char_pointer_size);
    if (input_parameter_list == NULL)
    {
        printf("%s: Memory Allocation Error.", __func__);
        abort();
    }

    // Save first parameter
    input_parameter_list[0]     = command_name;

    const char* const delimiter = " ";
    char* token                 = strtok(arguments, delimiter);
    input_parameter_list[1]     = token;

    for (uint32_t index = 2; index < number_of_arguments - 1; index++)
    {
        // Save arguments
        char* token                 = strtok(NULL, delimiter);
        input_parameter_list[index] = token;
        // printf("input_parameter_list[%d]=%s\n", index, input_parameter_list[index]);
    }
    input_parameter_list[number_of_arguments - 1] = NULL;

    return input_parameter_list;
}

/**
 * @brief Free the input parameter list created by create_input_parameter_list()
 */
void free_input_parameter_list(void* address)
{
    assert(address != NULL);

    free(address);
}