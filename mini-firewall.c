#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>


#define PROC_FILENAME "print_teste"
#define IPADDRESS(addr) \
	((unsigned char *)&addr)[3], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[0]
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

    // Copiar a mensagem do espaço do usuário para o buffer do kernel
    if (copy_from_user(&kernel_args, buff, len)) {
        printk(KERN_ERR "Falha ao copiar dados do espaço do usuário para o kernel\n");
        return -EFAULT;
    }

    printk(KERN_INFO "Protocol: %s, IP: %s, ACTION: %s\n",kernel_args.arg1, kernel_args.arg2, kernel_args.arg3);
    return len;
}

static const struct proc_ops printteste_ops = {
    .proc_write = printteste_write,
};


static char *ip_addr_rule = kernel_args.arg2;
static struct nf_hook_ops *nf_blockicmppkt_ops = NULL;
static struct nf_hook_ops *nf_blockipaddr_ops = NULL;


static unsigned int nf_blockipaddr_handler(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	if (!skb) {
		return NF_ACCEPT;
	} else {
		char *str = (char *)kmalloc(16, GFP_KERNEL);
		u32 sip;
		struct sk_buff *sb = NULL;
		struct iphdr *iph;

		sb = skb;
		iph = ip_hdr(sb);
		sip = ntohl(iph->saddr);
		
		sprintf(str, "%u.%u.%u.%u", IPADDRESS(sip));
		if(!strcmp(str, ip_addr_rule)) {
			if (strcmp(kernel_args.arg3, "ALLOW") == 0){
				return NF_ACCEPT;
			} else if (strcmp(kernel_args.arg3,"BLOCK") == 0) {
				return NF_DROP;
			} else {
                                return NF_ACCEPT;
			}	
		} else {
			return NF_ACCEPT;
		}
	}
}


static unsigned int nf_blockicmppkt_handler(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	struct iphdr *iph;
	struct udphdr *udph;
	if(!skb)
		return NF_ACCEPT;
	iph = ip_hdr(skb);
	if(iph->protocol == IPPROTO_UDP) {
		udph = udp_hdr(skb);
		if(ntohs(udph->dest) == 53) {
		        if (strcmp(kernel_args.arg1, "UDP") == 0 && strcmp(kernel_args.arg3, "ALLOW") == 0){
				return NF_ACCEPT;
			} else if (strcmp(kernel_args.arg1, "UDP") == 0 && strcmp(kernel_args.arg3,"BLOCK") == 0) {
				printk(KERN_INFO "Drop UDP packet\n");
				return NF_DROP;
			} else {
        	                return NF_ACCEPT;
			}
			
		}
	}
       	else if (iph->protocol == IPPROTO_TCP) {
	        if (strcmp(kernel_args.arg1, "TCP") == 0 && strcmp(kernel_args.arg3, "ALLOW") == 0){
			return NF_ACCEPT;
		} else if (strcmp(kernel_args.arg1, "TCP") == 0 && strcmp(kernel_args.arg3,"BLOCK") == 0) {
			printk(KERN_INFO "Drop TCP packet\n");
			return NF_DROP;
		} else {
                        return NF_ACCEPT;
		}


	}
       	else if (iph->protocol == IPPROTO_ICMP) {
		if (strcmp(kernel_args.arg1, "ICMP") == 0 && strcmp(kernel_args.arg3, "ALLOW") == 0){
			return NF_ACCEPT;
		} else if (strcmp(kernel_args.arg1, "ICMP") == 0 && strcmp(kernel_args.arg3,"BLOCK") == 0) {
			printk(KERN_INFO "Drop ICMP packet\n");
			return NF_DROP;
		} else {
                        return NF_ACCEPT;
		}

	}
	return NF_ACCEPT;
	
}

static int __init nf_minifirewall_init(void) {
	proc_create("print_teste", 0666, NULL, &printteste_ops);
	printk(KERN_INFO "Módulo do kernel carregado.\n");

	nf_blockicmppkt_ops = (struct nf_hook_ops*)kcalloc(1,  sizeof(struct nf_hook_ops), GFP_KERNEL);
	if (nf_blockicmppkt_ops != NULL) {
		nf_blockicmppkt_ops->hook = (nf_hookfn*)nf_blockicmppkt_handler;
		nf_blockicmppkt_ops->hooknum = NF_INET_PRE_ROUTING;
		nf_blockicmppkt_ops->pf = NFPROTO_IPV4;
		nf_blockicmppkt_ops->priority = NF_IP_PRI_FIRST;
		
		nf_register_net_hook(&init_net, nf_blockicmppkt_ops);
	}
	nf_blockipaddr_ops = (struct nf_hook_ops*)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);
	if (nf_blockipaddr_ops != NULL) {
		nf_blockipaddr_ops->hook = (nf_hookfn*)nf_blockipaddr_handler;
		nf_blockipaddr_ops->hooknum = NF_INET_PRE_ROUTING;
		nf_blockipaddr_ops->pf = NFPROTO_IPV4;
		nf_blockipaddr_ops->priority = NF_IP_PRI_FIRST + 1;

		nf_register_net_hook(&init_net, nf_blockipaddr_ops);
	}
	return 0;
}

static void __exit nf_minifirewall_exit(void) {
	remove_proc_entry(PROC_FILENAME, NULL);
	printk(KERN_INFO "Módulo do kernel descarregado.\n");

	if(nf_blockicmppkt_ops != NULL) {
		nf_unregister_net_hook(&init_net, nf_blockicmppkt_ops);
		kfree(nf_blockicmppkt_ops);
	}
	if (nf_blockipaddr_ops  != NULL) {
		nf_unregister_net_hook(&init_net, nf_blockipaddr_ops);
		kfree(nf_blockipaddr_ops);
	}
	printk(KERN_INFO "Exit");
}

module_init(nf_minifirewall_init);
module_exit(nf_minifirewall_exit);

MODULE_LICENSE("GPL");
