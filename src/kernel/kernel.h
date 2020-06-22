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


typedef asmlinkage long (*sys_creat_t)(const char __user* pathname, int mode);
typedef asmlinkage long (*sys_open_t)(const char __user* pathname, int flags, int mode);
typedef asmlinkage long (*sys_openat_t)(int dfd, const char __user* filename, int flags, int mode);
typedef asmlinkage long (*sys_write_t)( unsigned int fd, const char __user* buf, size_t count);
typedef asmlinkage long (*sys_writev_t)(unsigned long fd, const struct iovec __user* vec, unsigned long vlen);
typedef asmlinkage long (*sys_pwritev_t)(unsigned long fd, const struct iovec __user* vec, unsigned long vlen,  unsigned long pos_l, unsigned long pos_h);
typedef asmlinkage long (*sys_pwrite64_t)(unsigned int fd, const char __user* buf, size_t count, loff_t pos);
typedef asmlinkage long (*sys_mkdir_t)(const char __user* pathname, int mode);
typedef asmlinkage long (*sys_unlink_t)(const char __user* pathname);
typedef asmlinkage long (*sys_unlinkat_t)(int dfd, const char __user* pathname, int flag);
typedef asmlinkage long (*sys_link_t)(const char __user* oldname, const char __user* newname);
typedef asmlinkage long (*sys_linkat_t)(int olddfd, const char __user* oldname, int newdfd, const char __user* newname, int flags);
typedef asmlinkage long (*sys_symlink_t)(const char __user* oldname, const char __user* newname);
typedef asmlinkage long (*sys_symlinkat_t)(const char __user* oldname, int newdfd, const char __user* newname);
typedef asmlinkage long (*sys_rename_t)(const char __user* oldname, const char __user* newname);
typedef asmlinkage long (*sys_renameat_t)(int olddfd, const char __user* oldname,int newdfd, const char __user* newname);
typedef asmlinkage long (*sys_truncate_t)(const char __user* path, long length);
typedef asmlinkage long (*sys_ftruncate_t)(unsigned int fd, long length);
typedef asmlinkage long (*sys_rmdir_t)(const char __user* pathname);
typedef asmlinkage long (*sys_chown_t)(const char __user* filename, uid_t user, gid_t group);
typedef asmlinkage long (*sys_lchown_t)(const char __user* filename, uid_t user, gid_t group);
typedef asmlinkage long (*sys_fchown_t)(unsigned int fd, uid_t user, gid_t group);
typedef asmlinkage long (*sys_fchownat_t)(int dfd, const char __user* filename, uid_t user, gid_t group, int flag);
typedef asmlinkage long (*sys_chmod_t)(const char __user* filename, mode_t mode);
typedef asmlinkage long (*sys_fchmod_t)(unsigned int fd, mode_t mode);
typedef asmlinkage long (*sys_fchmodat_t)(int dfd, const char __user* filename, mode_t mode);
typedef asmlinkage long (*sys_close_t)(unsigned int fd);
//typedef asmlinkage long (*sys_io_submit_t)(aio_context_t, long, struct iocb __user * __user *);

//newsys.c
asmlinkage long new_create(const char __user* pathname, int mode);
asmlinkage long new_open(const char __user* pathname, int flags, int mode);
asmlinkage long new_openat(int dfd, const char __user* filename, int flags, int mode);
asmlinkage long new_write(unsigned int fd, const char __user* buf, size_t count);
asmlinkage long new_writev(unsigned long fd, const struct iovec __user* vec, unsigned long vlen);
asmlinkage long new_pwritev(unsigned long fd, const struct iovec __user* vec, unsigned long vlen,  unsigned long pos_l, unsigned long pos_h);
asmlinkage long new_pwrite64(unsigned int fd, const char __user* buf, size_t count, loff_t pos);
asmlinkage long new_mkdir(const char __user* pathname, int mode);
asmlinkage long new_unlink(const char __user* pathname);
asmlinkage long new_unlinkat(int dfd, const char __user* pathname, int flag);
asmlinkage long new_link(const char __user* oldname, const char __user* newname);
asmlinkage long new_linkat(int olddfd, const char __user* oldname, int newdfd, const char __user* newname, int flags);
asmlinkage long new_symlink(const char __user* oldname, const char __user* newname);
asmlinkage long new_symlinkat(const char __user* oldname, int newdfd, const char __user* newname);
asmlinkage long new_rename(const char __user* oldname, const char __user* newname);
asmlinkage long new_renameat(int olddfd, const char __user* oldname,int newdfd, const char __user* newname);
asmlinkage long new_truncate(const char __user* path, long length);
asmlinkage long new_ftruncate(unsigned int fd, long length);
asmlinkage long new_rmdir(const char __user *pathname);
asmlinkage long new_chown(const char __user* filename, uid_t user, gid_t group);
asmlinkage long new_lchown(const char __user* filename, uid_t user, gid_t group);
asmlinkage long new_fchown(unsigned int fd, uid_t user, gid_t group);
asmlinkage long new_fchownat(int dfd, const char __user* filename, uid_t user, gid_t group, int flag);
asmlinkage long new_chmod(const char __user* filename, mode_t mode);
asmlinkage long new_fchmod(unsigned int fd, mode_t mode);
asmlinkage long new_fchmodat(int dfd, const char __user* filename, mode_t mode);
asmlinkage long new_close(unsigned int fd);

//syshook.c
int syshook_init(void);
void syshook_exit(void);
