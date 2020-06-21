#include <linux/init.h>
#include <asm/unistd.h>
#include <linux/sched.h>
#include <linux/module.h>  
#include <linux/kernel.h>
#include "kernel.h"

#define HOOKSYS(function) \
do{ \
	old_##function = (sys_##function##_t)sys_call_table[__NR_##function]; \
	sys_call_table[__NR_##function] = new_##function; \
}while(0);

#define UNHOOKSYS(function)	\
do{ \
	if(old_##function != NULL) \
		sys_call_table[__NR_##function] = old_##function; \
}while(0);

static void **sys_call_table=NULL;
static unsigned int org_cr0 = 0;

sys_write_t old_write = NULL;
sys_mkdir_t old_mkdir = NULL;
sys_rmdir_t old_rmdir = NULL;

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

    retval = (unsigned long)memmem(sc_asm, 150, "\xff\x14\xc5", 3);
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
	printk("sshook init\n");

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

