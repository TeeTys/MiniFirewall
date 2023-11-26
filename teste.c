#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/proc/print_teste"

void print_usage(char *program_name) {
    fprintf(stderr, "Uso: %s --arg1 'arg1' --arg2 'arg2' --arg3 'arg3'...\n", program_name);
}

void trimTrailing(char * str);

struct UserArguments {
    char arg1[50];
    char arg2[50];
    char arg3[50];
};

int main(int argc, char *argv[]) {
    int fd;
    char buf[512];

    // Verify Arguments
    
    if (argc < 2 || (argc % 2) != 1){ 
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    } 

    // Open kernel module file
    fd = open(DEVICE_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Falha ao abrir o arquivo do módulo do kernel");
        exit(EXIT_FAILURE);
    }
    

    // Build the message to the kernel
    struct UserArguments args;
    snprintf(args.arg1, sizeof(args.arg1), "NULL");
    snprintf(args.arg2, sizeof(args.arg2), "NULL");
    snprintf(args.arg3, sizeof(args.arg3), "ALLOW");

    for (int i = 1; i < argc; i += 2){
    	if (strcmp(argv[i], "-P") == 0){
		snprintf(args.arg1, sizeof(args.arg1), argv[i+1]);
	} 
	if (strcmp(argv[i], "-I") == 0){
    		snprintf(args.arg2, sizeof(args.arg2), argv[i+1]);
	} 
	if (strcmp(argv[i], "-A") == 0){
	    	snprintf(args.arg3, sizeof(args.arg3), argv[i+1]);
	} 
    }

    // Write the message in kernel module file
   
    if (write(fd, &args, sizeof(args)) == -1) {
        perror("Falha ao escrever no arquivo do módulo do kernel");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Mensagem enviada para o módulo do kernel.\n");

    // Close kernel module file
    close(fd);

    return 0;
}
