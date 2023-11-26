// kernel_module.c

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>


#define PROC_FILENAME "print_teste"

struct KernelArguments {
    char arg1[50];
    char arg2[50];
    char arg3[50];
};

static struct KernelArguments kernel_args;

ssize_t printteste_write(struct file *filp, const char __user *buff, size_t len, loff_t *off) {

    
    if (len != sizeof(struct KernelArguments)) {
        printk(KERN_ERR "Tamanho da estrutura inválido.\n");
        return -EINVAL;
    }
    	
    // Copy massage from user space to the kernel
    if (copy_from_user(&kernel_args, buff, len)) {
        printk(KERN_ERR "Falha ao copiar dados do espaço do usuário para o kernel\n");
        return -EFAULT;
    }
    
    printk(KERN_INFO "arg1: %s, arg2: %s, arg3: %s\n",kernel_args.arg1, kernel_args.arg2, kernel_args.arg3);

    // Return the number of bytes written to the kernel buffer
    return len;
}

static const struct proc_ops printteste_ops = {
    .proc_write = printteste_write,
};

static int __init my_module_init(void) {
    // Create the file in the /proc directory
    proc_create("print_teste", 0666, NULL, &printteste_ops);
    printk(KERN_INFO "Módulo do kernel carregado.\n");
    return 0;
}

static void __exit my_module_exit(void) {
    // Remove the file from the /proc directory
    remove_proc_entry(PROC_FILENAME, NULL);
    printk(KERN_INFO "Módulo do kernel descarregado.\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Teety");
MODULE_DESCRIPTION("Exemplo de módulo do kernel que recebe mensagens do espaço do usuário");
