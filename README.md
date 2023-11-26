# CLI
This is an example of a reading comand line arguments program that prints those arugments in the kernel messages

# Kernel Module

1. Download "print_teste.c" and "Makefile" and put both in the same folder.
2. Run "sudo make" to compile the module
3. Run "sudo insmod print_teste.ko" to insert the module into to the kernel
4. Run "sudo lsmod | grep "print_teste" to check if the module was inserted

# CLI 

1. Download "teste.c"
2. Run "gcc teste.c -o teste" to compile and create output file "teste"
3. Run "./teste [Arguments] [Value]"

##  Usage

Run "./minifw [Arguments] [Value]"

Arguments: 

-P (Protocol) value: "TCP/UDP/ICMP" 

-I (Ip adress) value: "x.x.x.x"
           
-A (Action) value: "ALLOW/BLOCK"

Example: 

./teste -P "TCP" -A "BLOCK";

./teste -I "1.2.3.4" -A "BLOCK";
         
./teste -P "UDP" -A "ALLOW";
         
./teste -P "ICMP" -A "BLOCK";


