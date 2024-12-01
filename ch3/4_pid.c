/**
 * Kernel module that communicates with /proc file system.
 *
 * This provides the base logic for Project 2 - displaying task information
 */

/* Header */

#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

/* Macro */

/** We define these the same for all machines */
#define EXIT_FAILURE 1 /* Failing exit status */
#define EXIT_SUCCESS 0 /* Successful exit status */

#define BUFFER_SIZE  (128u)
#define PROC_NAME    "pid"

/* Type */

/* Prototype */

static ssize_t proc_read(struct file* file, char* buf, size_t count, loff_t* pos);
static ssize_t proc_write(struct file* file, const char __user* usr_buf, size_t count, loff_t* pos);

/* Variable */

/** The current pid */
static long l_pid;

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read  = proc_read,
};

/* Function */

/** This function is called when the module is loaded */
static int proc_init(void)
{
    /**
     * Creates the /proc/pid entry
     * The following function call is a wrapper for proc_create_data() passing NULL as the last argument
     */
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    return EXIT_SUCCESS;
}

/** This function is called when the module is removed */
static void proc_exit(void)
{
    // Removes the /proc/pid entry
    remove_proc_entry(PROC_NAME, NULL);

    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * @brief
 * This function is called each time the /proc/pid is read.
 * 
 * This function is called repeatedly until it returns 0, so there must be logic that ensures it ultimately returns 0 once it has collected the data that is to go into the corresponding /proc file.
 *
 * @param[in] file
 * @param[in] buf buffer in user space
 * @param[in] count
 * @param[in] pos
 */
static ssize_t proc_read(struct file* file, char __user* usr_buf, size_t count, loff_t* pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed    = 0;
    struct task_struct* tsk = NULL;

    if (completed)
    {
        completed = 0;
        return 0;
    }

    tsk       = pid_task(find_vpid(l_pid), PIDTYPE_PID);

    completed = 1;

    // Copies the contents of kernel buffer to userspace usr_buf
    if (copy_to_user(usr_buf, buffer, rv))
    {
        rv = -1;
    }

    return rv;
}

/**
 * This function is called each time we write to the /proc file system.
 */
static ssize_t proc_write(struct file* file, const char __user* usr_buf, size_t count, loff_t* pos)
{
    char* k_mem;

    // allocate kernel memory
    k_mem = kmalloc(count, GFP_KERNEL);

    /* copies user space usr_buf to kernel buffer */
    if (copy_from_user(k_mem, usr_buf, count))
    {
        printk(KERN_INFO "Error copying from user\n");
        return -1;
    }

    /**
 	 * kstrol() will not work because the strings are not guaranteed
	 * to be null-terminated.
	 * 
	 * sscanf() must be used instead.
	 */

    kfree(k_mem);

    return count;
}

/* Kernel module Setting */

/* Macros for registering module entry and exit points */
module_init(proc_init); // The module entry point, which represents the function that is invoked when the module is loaded into the kernel
module_exit(proc_exit); // The module exit point, which represents the function that is called when the module is removed from the kernel

/**
 * Details regarding
 * (1) Software license, 
 * (2) Description of the module 
 * (3) Author
 */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PID Module");
MODULE_AUTHOR("SGG");
