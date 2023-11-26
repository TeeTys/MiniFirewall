# MiniFirewall
Simple linux firewall example

# Kernel Module

1. Download "mini-firewall.c" and "Makefile" and put both in the same folder.
2. Run "sudo make" to compile the module
3. Run "sudo insmod mini-firewall.ko" to insert the module into to the kernel
4. Run "sudo lsmod | grep "mini-firewall" to check if the module was inserted

# CLI 

1. Download "minifw.c"
2. Run "gcc minifw.c -o minifw" to compile and create output file "minifw"
3. Run "./minifw [Arguments] [Value]"

##  Usage

Run "./minifw [Arguments] [Value]"

Arguments: -P (Protocol) value: "TCP/UDP/ICMP" 
           -I (Ip adress) value: "x.x.x.x"
           -A (Action) value: "ALLOW/BLOCK"

Example: ./minifw -P "TCP" -A "BLOCK";
         ./minifw -I "1.2.3.4" -A "BLOCK";
         ./minifw -P "UDP" -A "ALLOW";
         ./minifw -P "ICMP" -A "BLOCK";

