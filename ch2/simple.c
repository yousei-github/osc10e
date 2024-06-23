/**
 * simple.c
 *
 * A simple kernel module that illustrates the basic of kernel module
 * 
 * @note
 * The makefile must be modified to compile this program.
 * Add "simple.o" to the [obj-m] variable so the compiler knows what to build.
 * 
 * Kernel modules are loaded using the insmod command like,
 * [sudo insmod simple.ko]
 * 
 * To check whether the module has loaded, enter the [lsmod] command and search for the module simple.
 * 
 * To check the contents of this message in the kernel log buffer, enter the command
 * [sudo dmesg]
 * You should see the message "Loading the Simple Module."
 * 
 * Because the kernel log buffer can fill up quickly, it often makes sense to clear the buffer periodically:
 * [sudo dmesg -c]
 * 
 * The compilation produces several files. The file simple.ko represents the compiled kernel module.
 * 
 * Removing the kernel module involves invoking the rmmod command (notice that the .ko suffix is unnecessary):
 * [sudo rmmod simple]
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

/* Header */
#include <asm/param.h>
#include <linux/gcd.h>
#include <linux/hash.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>

/* Macro */

/** @brief For Timer */
#define ONE_SECOND_IN_MILLISECOND (1000u)
#define ONE_SECOND_IN_MICROSECOND (1000000u)

#define CLOCK_RATE_THRESHOLD      (1000u)

#ifdef HZ

#if (HZ < CLOCK_RATE_THRESHOLD)

#define ONE_SECOND_COUNT ONE_SECOND_IN_MILLISECOND
#define TIME_UNIT        "ms"

#else

#define TIME_UNIT        "us"
#define ONE_SECOND_COUNT ONE_SECOND_IN_MICROSECOND

#endif /* HZ < CLOCK_RATE_THRESHOLD */

#else
#error "Please include the header <asm/param.h>"
#endif /* HZ */

/* Type */

/* Prototype */

/* Variable */

unsigned long startTime, endTime; // unit is us

/* Function */

/* This function is called when the module is loaded. */
static int simple_init(void)
{
    /** 
     * @brief
     * Function printk() is the kernel equivalent of printf()
     * 
     * Its output is sent to a kernel log buffer whose contents can be read by the [dmesg] command.
     */
    printk(KERN_INFO "Loading the Simple Module\n");

    printk(KERN_INFO "Golden ratio prime is %llu\n", GOLDEN_RATIO_PRIME);

    printk(KERN_INFO "The greatest common divisor of 3300 and 24 is %lu\n", gcd(3300, 24));

    printk(KERN_INFO "The tick rate is %u Hz\n", HZ);

    startTime = jiffies * (ONE_SECOND_COUNT / HZ);

    printk(KERN_INFO "The start time is %lu %s\n", startTime, TIME_UNIT);

    return 0;
}

/* This function is called when the module is removed. */
static void simple_exit(void)
{
    endTime = jiffies * (ONE_SECOND_COUNT / HZ);

    printk(KERN_INFO "The end time is %lu %s, elapsed time is %lu %s\n", endTime, TIME_UNIT, endTime - startTime, TIME_UNIT);

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
