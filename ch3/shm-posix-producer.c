/**
 * @brief POSIX Shared Memory
 * 
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 * It creates a shared-memory object named "OS" and writes the string "Studying Operating Systems Is Fun!" to shared memory.
 * The program memory-maps a shared-memory object of the specified size and allows writing to the object.
 *
 * Figure 3.16
 * 
 * To compile, enter
 * 	gcc shm-posix-producer.c -lrt
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

/* Header */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Macro */

/* Type */

/* Prototype */

/* Variable */

/* Function */

int main()
{
    // POSIX shared memory is organized using memory-mapped files, which associate the region of shared memory with a file.

    const int SIZE       = 4096;
    const char* name     = "OS";
    const char* message0 = "Studying ";
    const char* message1 = "Operating Systems ";
    const char* message2 = "Is Fun!\n";

    /** 
	 * A process must first create a shared-memory object (or shared memory segment) using the shm_open() system call
	 * 
	 * The first parameter specifies the name of the shared-memory object.
	 * Processes that wish to access this shared memory must refer to the object by this name.
	 * The subsequent parameters specify that the shared-memory object is to be created
	 * if it does not yet exist (O_CREAT) and that the object is open for reading and writing (O_RDWR).
	 * The last parameter establishes the file-access permissions of the shared-memory object.
	 * 
	 * A successful call to shm_open() returns an integer file descriptor for the shared-memory object.
	 */
    const int shm_fd     = shm_open(name, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1)
    {
        printf("Open shared memory segment failed\n");
        exit(-1);
    }
    printf("Open shared memory segment successful, file descriptor is %d\n", shm_fd);

    /** Configure the size of the shared-memory object (or shared memory segment) */
    ftruncate(shm_fd, SIZE);

    /**
	 * Now map the shared memory segment in the address space of the process via a memory-mapped file.
	 * Note the mmap() function returns a pointer to the memory-mapped file that is used for accessing the shared-memory object.
	 * 
	 * The flag MAP_SHARED specifies that changes to the shared-memory object will be visible to all processes sharing the object.
	 */
    void* ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        return -1;
    }

    /**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
    sprintf(ptr, "%s", message0);
    ptr += strlen(message0);
    sprintf(ptr, "%s", message1);
    ptr += strlen(message1);
    sprintf(ptr, "%s", message2);
    ptr += strlen(message2);

    return EXIT_SUCCESS;
}
