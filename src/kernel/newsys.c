#include "kernel.h"

extern sys_write_t old_write;
extern sys_mkdir_t old_mkdir;
extern sys_rmdir_t old_rmdir;


asmlinkage long new_mkdir(const char __user* pathname, int mode){
	printk("new_mkdir\n");
	return old_mkdir(pathname,mode);
}

asmlinkage long new_rmdir(const char __user* pathname){
	printk("new_rmdir\n");
	return old_rmdir(pathname);
}

asmlinkage long new_write(unsigned int fd, const char __user* buf, size_t count){
//	printk("nsys_write\n");
	return old_write(fd, buf, count);
}
