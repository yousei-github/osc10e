/**
 * hello.c
 *
 * Kernel module that communicates with /proc file system.
 * 
 * @note
 * The makefile must be modified to compile this program.
 * Add "hello.o" to the [obj-m] variable so the compiler knows what to build.
 *
 * The /proc file system is a "pseudo" file system that exists only in kernel memory,
 * and is used primarily for querying various kernel and per-process statistics.
 * 
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#define BUFFER_SIZE 128

#define PROC_NAME   "hello"
#define MESSAGE     "Hello World\n"

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file* file, char* buf, size_t count, loff_t* pos);

// static struct file_operations proc_ops = {
//     .owner = THIS_MODULE,
//     .read  = proc_read,
// };

static struct proc_ops proc_ops = {
    .proc_flags = 0,
    .proc_read  = proc_read,
};

/* This function is called when the module is loaded. */
static int proc_init(void)
{
    /** 
     * Creates the /proc/hello entry
     * The following function call is a wrapper for proc_create_data() passing NULL as the last argument
     */
    proc_create(PROC_NAME, 0, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{
    // Removes the /proc/hello entry
    remove_proc_entry(PROC_NAME, NULL);

    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/hello is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
static ssize_t proc_read(struct file* file, char __user* usr_buf, size_t count, loff_t* pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed)
    {
        completed = 0;
        return 0;
    }

    completed                = 1;

    rv                       = sprintf(buffer, "Hello World\n");

    // Copies the contents of buffer to userspace usr_buf
    const unsigned int error = copy_to_user(usr_buf, buffer, rv);

    if (error)
    {
        printk(KERN_INFO "Copy Error: error code is %u\n", error);
    }

    return rv;
}

/* Macros for registering module entry and exit points. */
module_init(proc_init); // The module entry point, which represents the function that is invoked when the module is loaded into the kernel
module_exit(proc_exit); // The module exit point, which represents the function that is called when the module is removed from the kernel

/**
 * Details regarding
 * (1) Software license, 
 * (2) Description of the module 
 * (3) Author
 */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");
