#include <linux/init.h>
#include <asm/unistd.h>
#include <linux/sched.h>
#include <linux/module.h>  
#include <linux/kernel.h>
#include "kernel.h"

sys_creat_t     old_create      = NULL;
sys_open_t      old_open        = NULL;
sys_openat_t    old_openat      = NULL;
sys_write_t     old_write       = NULL;
sys_writev_t    old_writev      = NULL;
sys_pwritev_t   old_pwritev     = NULL;
sys_pwrite64_t  old_pwrite64    = NULL;
sys_mkdir_t     old_mkdir       = NULL;
sys_unlink_t    old_unlink      = NULL;
sys_unlinkat_t  old_unlinkat    = NULL;
sys_link_t      old_link        = NULL;
sys_linkat_t    old_linkat      = NULL;
sys_symlink_t   old_symlink     = NULL;
sys_symlinkat_t old_symlinkat   = NULL;
sys_rename_t    old_rename      = NULL;
sys_renameat_t  old_renameat    = NULL;
sys_truncate_t  old_truncate    = NULL;
sys_ftruncate_t old_ftruncate   = NULL;
sys_rmdir_t     old_rmdir       = NULL;
sys_chown_t     old_chown       = NULL;
sys_lchown_t    old_lchown      = NULL;
sys_fchown_t    old_fchown      = NULL;
sys_fchownat_t  old_fchownat    = NULL;
sys_chmod_t     old_chmod       = NULL;
sys_fchmod_t    old_fchmod      = NULL;
sys_fchmodat_t  old_fchmodat    = NULL;
sys_close_t     old_close       = NULL;


#define HOOKSYS(function)   do{ old_##function = (sys_##function##_t)sys_call_table[__NR_##function];	sys_call_table[__NR_##function] = new_##function; }while(0);
#define UNHOOKSYS(function) do{ if(old_##function != NULL) sys_call_table[__NR_##function] = old_##function; }while(0);

static void **sys_call_table=NULL;
static unsigned int org_cr0 = 0;

struct _idtr {
    unsigned short limit;
    unsigned long base;
} idtr;

typedef struct _idt{
    u16 offset_low;
    u16 segment;
    unsigned ist : 3, zero0 : 5, type : 5, dpl :2, p : 1;
    u16 offset_middle;
    u32 offset_high;
    u32 zero1;
}idt;

static void setback_cr0(unsigned long val){
	__asm__ __volatile__
	(
		"movq %%rax, %%cr0"
		::"a"(val)
	);
}

static unsigned long clear_and_return_origcr0(void){
	unsigned long cr0 = 0;
	unsigned ret;

	asm volatile ("mov %%cr0,%0;clts" : "=r"(cr0));
	ret = cr0;
	cr0 &= 0xfffffffffffeffff;

	__asm__ __volatile__
	(
		"movq %%rax,%%cr0"
		::"a"(cr0)
	);
	return ret;
}

static void *memmem(const void *haystack, size_t haystack_len, const void *needle, size_t needle_len){
    const char *begin;
    const char *const last_possible = (const char *) haystack + haystack_len - needle_len;

    if (needle_len == 0)
        return (void *) haystack;

    if (__builtin_expect(haystack_len < needle_len, 0))
        return NULL;

    for (begin = (const char *) haystack; begin <= last_possible; ++begin){
        if (begin[0] == ((const char *) needle)[0] && !memcmp(begin + 1, needle + 1, needle_len - 1)){
            return (void *) begin;
        }
    }

    return NULL;
}

static void *getsystable(void){
    unsigned long syscall_long, retval;
    char sc_asm[150];

    rdmsrl(MSR_LSTAR, syscall_long);

    memcpy(sc_asm, (char*)syscall_long, 150);

    retval = (unsigned long)memmem(sc_asm, 150, "\xff\x14\xc5", 3);  //find 'callq'
    if ( retval != 0 ){
        retval = (unsigned long)(*(unsigned long*)(retval+3));
    }else{
        printk("getsystable fail!\n");
        retval = 0;
    }

    retval |= 0xffffffff00000000;

    return (void *)retval;
}

int  syshook_init(void){
	printk("syshook init\n");

	sys_call_table = (void**)getsystable();
	if(sys_call_table == NULL)
		return -1;

	org_cr0 = clear_and_return_origcr0();

	HOOKSYS(write);
	HOOKSYS(mkdir);
	HOOKSYS(rmdir);

	setback_cr0(org_cr0);

	return 0;
}

void syshook_exit(void){
	org_cr0 = clear_and_return_origcr0();

	UNHOOKSYS(write);
	UNHOOKSYS(mkdir);
	UNHOOKSYS(rmdir);

	setback_cr0(org_cr0);

	printk("syshook exit\n");
}

asmlinkage long new_create(const char __user* pathname, int mode){
	long ret = 0;
	ret = old_create(pathname, mode);
	return ret;
}

asmlinkage long new_open(const char __user* pathname, int flags, int mode){
	long ret = 0;
	ret = old_open(pathname, flags, mode);
	return ret;
}

asmlinkage long new_openat(int dfd, const char __user* filename, int flags, int mode){
	long ret = 0;
	ret = old_openat(dfd, filename, flags, mode);
	return ret;
}

asmlinkage long new_write(unsigned int fd, const char __user* buf, size_t count){
	long ret = 0;
	ret = old_write(fd, buf, count);
	return ret;
}

asmlinkage long new_writev(unsigned long fd, const struct iovec __user* vec, unsigned long vlen){
	long ret = 0;
	ret = old_writev(fd, vec, vlen);
	return ret;
}

asmlinkage long new_pwritev(unsigned long fd, const struct iovec __user* vec, unsigned long vlen,  unsigned long pos_l, unsigned long pos_h){
	long ret = 0;
	ret = old_pwritev(fd, vec, vlen, pos_l, pos_h);
	return ret;
}

asmlinkage long new_pwrite64(unsigned int fd, const char __user* buf, size_t count, loff_t pos){
	long ret = 0;
	ret = old_pwrite64(fd, buf, count, pos);
	return ret;
}

asmlinkage long new_mkdir(const char __user* pathname, int mode){
	long ret = 0;
	ret = old_mkdir(pathname, mode);
	return ret;
}

asmlinkage long new_unlink(const char __user* pathname){
	long ret = 0;
	ret = old_unlink(pathname);
	return ret;
}

asmlinkage long new_unlinkat(int dfd, const char __user* pathname, int flag){
	long ret = 0;
	ret = old_unlinkat(dfd, pathname, flag);
	return ret;
}

asmlinkage long new_link(const char __user* oldname, const char __user* newname){
	long ret = 0;
	ret = old_link(oldname, newname);
	return ret;
}

asmlinkage long new_linkat(int olddfd, const char __user* oldname, int newdfd, const char __user* newname, int flags){
	long ret = 0;
	ret = old_linkat(olddfd, oldname, newdfd, newname, flags);
	return ret;
}

asmlinkage long new_symlink(const char __user* oldname, const char __user* newname){
	long ret = 0;
	ret = old_symlink(oldname, newname);
	return ret;
}

asmlinkage long new_symlinkat(const char __user* oldname, int newdfd, const char __user* newname){
	long ret = 0;
	ret = old_symlinkat(oldname, newdfd, newname);
	return ret;
}

asmlinkage long new_rename(const char __user* oldname, const char __user* newname){
	long ret = 0;
	ret = old_rename(oldname, newname);
	return ret;
}

asmlinkage long new_renameat(int olddfd, const char __user* oldname,int newdfd, const char __user* newname){
	long ret = 0;
	ret = old_renameat(olddfd, oldname, newdfd, newname);
	return ret;
}

asmlinkage long new_truncate(const char __user* path, long length){
	long ret = 0;
	ret = old_truncate(path, length);
	return ret;
}

asmlinkage long new_ftruncate(unsigned int fd, long length){
	long ret = 0;
	ret = old_ftruncate(fd, length);
	return ret;
}

asmlinkage long new_rmdir(const char __user* pathname){
	long ret = 0;
	ret = old_rmdir(pathname);
	return ret;
}

asmlinkage long new_chown(const char __user* filename, uid_t user, gid_t group){
	long ret = 0;
	ret = old_chown(filename, user, group);
	return ret;
}

asmlinkage long new_lchown(const char __user* filename, uid_t user, gid_t group){
	long ret = 0;
	ret = old_lchown(filename, user, group);
	return ret;
}

asmlinkage long new_fchown(unsigned int fd, uid_t user, gid_t group){
	long ret = 0;
	ret = old_fchown(fd, user, group);
	return ret;
}

asmlinkage long new_fchownat(int dfd, const char __user* filename, uid_t user, gid_t group, int flag){
	long ret = 0;
	ret = old_fchownat(dfd, filename, user, group, flag);
	return ret;
}

asmlinkage long new_chmod(const char __user* filename, mode_t mode){
	long ret = 0;
	ret = old_chmod(filename, mode);
	return ret;
}

asmlinkage long new_fchmod(unsigned int fd, mode_t mode){
	long ret = 0;
	ret = old_fchmod(fd, mode);
	return ret;
}

asmlinkage long new_fchmodat(int dfd, const char __user* filename, mode_t mode){
	long ret = 0;
	ret = old_fchmodat(dfd, filename, mode);
	return ret;
}

asmlinkage long new_close(unsigned int fd){
	long ret = 0;
	ret = old_close(fd);
	return ret;
}

