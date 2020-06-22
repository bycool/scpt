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

