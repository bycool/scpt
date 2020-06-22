#include "kernel.h"

extern	sys_creat_t     old_create      ;
extern	sys_open_t      old_open        ;
extern	sys_openat_t    old_openat      ;
extern	sys_write_t     old_write       ;
extern	sys_writev_t    old_writev      ;
extern	sys_pwritev_t   old_pwritev     ;
extern	sys_pwrite64_t  old_pwrite64    ;
extern	sys_mkdir_t     old_mkdir       ;
extern	sys_unlink_t    old_unlink      ;
extern	sys_unlinkat_t  old_unlinkat    ;
extern	sys_link_t      old_link        ;
extern	sys_linkat_t    old_linkat      ;
extern	sys_symlink_t   old_symlink     ;
extern	sys_symlinkat_t old_symlinkat   ;
extern	sys_rename_t    old_rename      ;
extern	sys_renameat_t  old_renameat    ;
extern	sys_truncate_t  old_truncate    ;
extern	sys_ftruncate_t old_ftruncate   ;
extern	sys_rmdir_t     old_rmdir       ;
extern	sys_chown_t     old_chown       ;
extern	sys_lchown_t    old_lchown      ;
extern	sys_fchown_t    old_fchown      ;
extern	sys_fchownat_t  old_fchownat    ;
extern	sys_chmod_t     old_chmod       ;
extern	sys_fchmod_t    old_fchmod      ;
extern	sys_fchmodat_t  old_fchmodat    ;
extern	sys_close_t     old_close       ;


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

