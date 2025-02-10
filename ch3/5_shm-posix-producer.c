/**
 * @file
 * shm-posix-producer.c
 * 
 * @brief
 * POSIX Shared Memory
 * 
 * @details
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 * It creates a shared-memory object named "OS" and writes the string "Studying Operating Systems Is Fun!" to shared memory.
 * The program memory-maps a shared-memory object of the specified size and allows writing to the object.
 *
 * Figure 3.16 Producer process illustrating POSIX shared-memory API.
 * 
 * To compile, enter
 * 	gcc shm-posix-producer.c -lrt
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#ifndef __linux__
#error "Must be using Linux kernel!"
#endif /* __linux__ */

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

    const char* const name     = "OS";
    const int SIZE             = 4096;
    const char* const message0 = "Studying ";
    const char* const message1 = "Operating Systems ";
    const char* const message2 = "Is Fun!\n";

    /** 
	 * (1) A process must first create a shared-memory object (or shared memory segment) using the shm_open() system call
	 * 
     * @details
	 * - The first parameter specifies the name of the shared-memory object.
	 * Processes that wish to access this shared memory must refer to the object by this name.
	 * 
     * - The subsequent parameters specify that the shared-memory object is to be created if it does not yet exist (O_CREAT) and that the object is open for reading and writing (O_RDWR).
	 * 
     * - The last parameter establishes the file-access permissions of the shared-memory object.
     * File and directory permissions in octal mode are based in binary. Each permission type is treated as a bit that is either set to off (0) or on (1).
     * In permissions, order has meaning. Permissions are always in read (r), write (w), and execute (x) order. If r, w, and x are all set to off, the binary representation is 000.
     * If they are all set to on, the binary representation is 111.
     * 
     * Convert the binary representation into base 10 (decimal) to use in chmod command, and so on. The shortcut here is to remember that read equals 4, write equals 2, and execute equals 1.
	 * The permissions number is determined by adding up the values for each permission type. There are eight possible values from zero to seven, hence the name octal mode.
     * 
     * This table demonstrates all eight of the possible permutations:
     * |Octal|Binary|String|Description|
     * |0    |000   |---   |No permissions|
     * |1    |001   |--x   |Execute only|
     * |2    |010   |-w-   |Write only|
     * |3    |011   |-wx   |Write and execute (2 + 1)|
     * |4    |100   |r--   |Read only|
     * |5    |101   |r-x   |Read and execute (4 + 1)|
     * |6    |110   |rw-   |Read and write (4 + 2)|
     * |7    |111   |rwx   |Read, write, and execute (4 + 2 + 1)|
     * 
     * And, in permissions order has meaning. The user categories are always in user, group, and other order.
     * 
	 * - A successful call to shm_open() returns an integer file descriptor for the shared-memory object.
	 */
    const int shm_fd           = shm_open(name, O_CREAT | O_RDWR, 0660);

    if (shm_fd == -1)
    {
        printf("Open shared memory segment failed\n");
        exit(-1);
    }
    printf("Open shared memory segment successful, file descriptor is %d\n", shm_fd);

    /** (2) Configure the size of the shared-memory object (or shared memory segment) */
    ftruncate(shm_fd, SIZE);

    /**
	 * (3) Now map the shared-memory object (shared memory segment) in the address space of the process via a memory-mapped file.
	 * Note the mmap() function returns a pointer to the memory-mapped file that is used for accessing the shared-memory object.
	 * 
	 * The flag MAP_SHARED specifies that changes to the shared-memory object will be visible to all processes sharing the object.
	 */
    void* ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        return EXIT_FAILURE;
    }

    /**
	 * (4) Now write to the shared memory region.
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
