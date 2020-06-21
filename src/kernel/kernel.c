#include "kernel.h"

static int __init scpt_init(void){
	printk("scpt_init\n");
	syshook_init();
	return 0;
}

static void __exit scpt_exit(void){
	syshook_exit();
    printk("scpt_exit\n");
}

module_init(scpt_init);
module_exit(scpt_exit);

MODULE_LICENSE("GPL");

