#ifndef __fs_h__
#define __fs_h__
#include <typedef.h>
#include <fs/vfs.h>

int read_iso9660(int devnum, const char *path, void *vfs,uint8_t flg);

//vfs.c
_Bool vfs_read(void *out,char *path);
_Bool vfs_write(void *in,char *path);

int vfs_read_directory(char *out,const char *path);
int rdir(const char *path);
int cd(const char *path);
#endif
