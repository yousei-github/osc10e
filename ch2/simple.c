/**
 * simple.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make".
 * 
 * @note
 * The makefile must be modified to compile this program.
 * Add "simple.o" to the [obj-m] variable
 * 
 * Kernel modules are loaded using the insmod command like,
 * sudo insmod simple.ko
 * 
 * To check whether themodule has loaded, enter the lsmod command and search for the module simple.
 * 
 * To check the contents of this message in the kernel log buffer, enter the command
 * sudo dmesg
 * You should see the message "Loading the Simple Module."
 * 
 * Removing the kernel module involves invoking the rmmod command (notice that the .ko suffix is unnecessary):
 * sudo rmmod simple
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/* This function is called when the module is loaded. */
static int simple_init(void)
{
    // Function printk() is the kernel equivalent of printf()
    printk(KERN_INFO "Loading the Simple Module\n");

    return 0;
}

/* This function is called when the module is removed. */
static void simple_exit(void)
{
    printk(KERN_INFO "Removing the Simple Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init); // The module entry point, which represents the function that is invoked when the module is loaded into the kernel
module_exit(simple_exit); // The module exit point, which represents the function that is called when the module is removed from the kernel

/**
 * Details regarding
 * (1) Software license, 
 * (2) Description of the module 
 * (3) Author
 */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
