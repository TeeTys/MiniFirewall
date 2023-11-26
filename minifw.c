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

    // Verificar se a quantidade de argumentos está correta
    
    if (argc < 2 || (argc % 2) != 1){ //|| strcmp(argv[1], "--P") != 0 || strcmp(argv[3], "--I") != 0 || strcmp(argv[5],"--A") != 0) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    } 

    // Abrir o arquivo do módulo do kernel
    fd = open(DEVICE_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Falha ao abrir o arquivo do módulo do kernel");
        exit(EXIT_FAILURE);
    }
    

    // Construir a mensagem a ser enviada para o módulo do kernel
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
//  if (strcmp(argv[6],"ALLOW") != 0 && strcmp(argv[6],"BLOCK") != 0) {
//    if (strcmp(args.arg1,"") == 0) {
//	snprintf(args.arg1, sizeof(args.arg3), "NULL");
//    }	

//    if (strcmp(args.arg2,"") == 0) {
//	snprintf(args.arg2, sizeof(args.arg3), "NULL");
//    }

//    if (strcmp(args.arg3,"") == 0) {
//	snprintf(args.arg3, sizeof(args.arg3), "NULL");
//    }

    //buf[0] = '\0';
    //for (int i = 1; i < argc; i += 2){
    //	snprintf(buf + strlen(buf), sizeof(buf)-strlen(buf), "%s ", argv[i+1]);
    //}
    // Escrever a mensagem no arquivo do módulo do kernel
    //printf("%s \n", buf);
    //trimTrailing(buf);
    if (write(fd, &args, sizeof(args)) == -1) {
        perror("Falha ao escrever no arquivo do módulo do kernel");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Mensagem enviada para o módulo do kernel.\n");

    // Fechar o arquivo do módulo do kernel
    close(fd);

    return 0;
}

void trimTrailing(char * str)
{
    int index, i;

    /* Set default index */
    index = -1;

    /* Find last index of non-white space character */
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }

        i++;
    }

    /* Mark next character to last non-white space character as NULL */
    str[index + 1] = '\0';
}
