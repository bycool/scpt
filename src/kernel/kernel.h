#include <linux/version.h>
#include <asm/unistd.h>
#include <linux/module.h>
#include <linux/file.h>
#include <linux/syscalls.h>
#include <linux/kthread.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/dcache.h>
#include <linux/sched.h>
#include <linux/uio.h>
#include <linux/slab.h>
#include <linux/namei.h>
#include <linux/nfs_fs.h>
#include <linux/audit.h>
#include <linux/mount.h>
#include <linux/fs_struct.h>

typedef asmlinkage long (*sys_open_t)(const char __user* pathname, int flags, int mode);
typedef asmlinkage long (*sys_write_t)(unsigned int fd, const char __user* buf, size_t count);
typedef asmlinkage long (*sys_mkdir_t)(const char __user* pathname, int mode);
typedef asmlinkage long (*sys_rmdir_t)(const char __user *pathname);

//syshook.c
int syshook_init(void);
void syshook_exit(void);


//newsys.c
asmlinkage long new_open(const char __user* pathname, int flags, int mode);
asmlinkage long new_write(unsigned int fd, const char __user* buf, size_t count);
asmlinkage long new_mkdir(const char __user* pathname, int mode);
asmlinkage long new_rmdir(const char __user *pathname);

