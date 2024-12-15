/**
 * @brief POSIX Shared Memory
 * 
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process reads and outputs the contents of the shared memory.
 *
 * Figure 3.17 Consumer process illustrating POSIX shared-memory API.
 *
 * To compile, enter
 *	gcc shm-posix-consumer.c -lrt
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

/* Header */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

/* Macro */

/* Type */

/* Prototype */

/* Variable */

/* Function */

int main()
{
    const char* name = "OS";
    const int SIZE   = 4096;

    /** (1) Open the shared memory segment */
    const int shm_fd = shm_open(name, O_RDONLY, 0440);
    if (shm_fd == -1)
    {
        printf("Open shared memory failed\n");
        exit(-1);
    }
    printf("Open shared memory segment successful, file descriptor is %d\n", shm_fd);

    /** (2) Now map the shared memory segment in the address space of the process */
    void* ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        exit(-1);
    }

    /** (3) Now read from the shared memory region */
    printf("%s", (char*) ptr);

    /** (4) Remove the shared memory segment by invoking the shm_unlink() */
    if (shm_unlink(name) == -1)
    {
        printf("Error removing %s\n", name);
        exit(-1);
    }

    return EXIT_SUCCESS;
}
