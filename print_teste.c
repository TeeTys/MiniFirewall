// kernel_module.c

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>


#define PROC_FILENAME "print_teste"
//#define BUFFER_SIZE 512
//#define MAX_ARGS 32

//static char proc_buffer[BUFFER_SIZE];
struct KernelArguments {
    char arg1[50];
    char arg2[50];
    char arg3[50];
};

static struct KernelArguments kernel_args;

ssize_t printteste_write(struct file *filp, const char __user *buff, size_t len, loff_t *off) {
//    char* token, *str, *tofree;
//    char *args[MAX_ARGS];
//    int num_args = 0;
      // Limpar o buffer antes de receber a nova mensagem
//    memset(proc_buffer, 0, sizeof(proc_buffer));
    
    if (len != sizeof(struct KernelArguments)) {
        printk(KERN_ERR "Tamanho da estrutura inválido.\n");
        return -EINVAL;
    }
    	
    // Copiar a mensagem do espaço do usuário para o buffer do kernel
    if (copy_from_user(&kernel_args, buff, len)) {
        printk(KERN_ERR "Falha ao copiar dados do espaço do usuário para o kernel\n");
        return -EFAULT;
    }
    
    printk(KERN_INFO "arg1: %s, arg2: %s, arg3: %s\n",kernel_args.arg1, kernel_args.arg2, kernel_args.arg3);

//    tofree = str = kstrdup(proc_buffer, GFP_KERNEL);
//    if (!str) {
//        printk(KERN_ERR "Falha ao alocar memória para a cópia da string\n");
//        return -ENOMEM;
//    }

     // Dividir a mensagem em argumentos usando espaços como delimitadores
//     while ((token = strsep(&str, " ")) != NULL && num_args < MAX_ARGS) {
//	printk(KERN_INFO "Token: %s\n", token);
          // Armazenar cada argumento no array
//        args[num_args++] = token;
//    }

//    kfree(tofree);  // Liberar a memória alocada para a cópia da string


    // Imprimir cada argumento separadamente
//    printk(KERN_INFO "Mensagem recebida do espaço do usuário: %s\n", proc_buffer);
//    printk(KERN_INFO "Número total de argumentos: %d\n", num_args);    
//    printk(KERN_INFO "Arg1: %s", args[0]);
//    printk(KERN_INFO "Arg2: %s", args[1]);
//    printk(KERN_INFO "Arg3: %s", args[2]);
//    for (int i = 0; i < num_args; ++i) {
//        printk(KERN_INFO "Argumento %d: %s \n", i+1, args[i]);
//    }


    // Retornar a quantidade de bytes escrita no buffer do kernel
    return len;
}

static const struct proc_ops printteste_ops = {
    .proc_write = printteste_write,
};

static int __init my_module_init(void) {
    // Criar o arquivo no diretório /proc
    proc_create("print_teste", 0666, NULL, &printteste_ops);
    printk(KERN_INFO "Módulo do kernel carregado.\n");
    return 0;
}

static void __exit my_module_exit(void) {
    // Remover o arquivo do diretório /proc
    remove_proc_entry(PROC_FILENAME, NULL);
    printk(KERN_INFO "Módulo do kernel descarregado.\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Teety");
MODULE_DESCRIPTION("Exemplo de módulo do kernel que recebe mensagens do espaço do usuário");
