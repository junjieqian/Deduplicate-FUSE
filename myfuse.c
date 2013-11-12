/* myfuse.c
 * Junjie Qian, jqian@cse.unl.edu
 * Build one fuse system
 */

#define FUSE_USE_VERSION 26

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/xattr.h>

static char *root;

static void my_fullpath(char fullpath[PATH_MAX], const char *path)
{
	strcpy(fullpath, root);
	strncat(fullpath, path, PATH_MAX);
}

// check the file access permissions, using the access() system call
static int my_access(const char *path, int mask)
{
	int res = 0;
	char fullpath[PATH_MAX];

	my_fullpath(fullpath, path);

	res = access(fullpath, mask);

	return res;
}

// change the permission mode of the file
static int my_chmod(const char *path, mode_t mode)
{
	int res = 0;
	char fullpath[PATH_MAX];

	my_fullpath(fullpath, path);

	res = chmod(fullpath, mode);
	return res;
}

// change the owner/group of the file
static int my_chown(const char *path, uid_t uid, gid_t gid)
{
	int res = 0;
	char fullpath[PATH_MAX];

	my_fullpath(fullpath, path);

	res = chown(fullpath, uid, gid);
	return res;
}

// create one file, using create syscall.
static int my_create(const char *path, mode_t mode, struct fuse_file_info *file)
{
	int res = 0;
	char fullpath[PATH_MAX];
	my_fullpath(fullpath, path);

	res = creat(fullpath, mode);
	file->fh = res;

	return res;
}

static int my_getattr(const char *path, struct stat *statbuf)
{
	int res = 0;
	char fullpath[PATH_MAX];

	my_fullpath(fullpath, path);

	res = lstat(fullpath, statbuf);

	return res;
}

// open directory
static int my_open(const char *path, struct fuse_file_info *file)
{
	int res = 0;
	char fullpath[PATH_MAX];

	my_fullpath(fullpath, path);

	res = open(fullpath, file->flags);

	return res;
}

// read directory
static int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler, 
	off_t offset, struct fuse_file_info *file)
{
	int res = 0;
	DIR *dir = (DIR*)file->fh;
	struct dirent *de = readdir(dir);

	if (de == NULL)
		return res;
	else
		do{
			if(filler(buf, de->d_name, NULL, 0) != 0)
				return -ENOMEM;
		} while((de = readdir(dir)) != NULL);

	return res;
}

// release directory
static int my_release(const char *path, struct fuse_file_info *file)
{
	closedir((DIR *)file->fh);
	return 0;
}

// delete directory
static int my_rmdir(const char *path)
{
	int res = 0;
	char fullpath[PATH_MAX];

	my_fullpath(fullpath, path);

	res = rmdir(fullpath);
	return res;
}

// make directory
static int my_mkdir(const char *path, mode_t mode)
{
	int res = 0;
	char fullpath[PATH_MAX];

	my_fullpath(fullpath, path);

	res = mkdir(fullpath, mode);
	return res;
}

// remove one file
static int my_unlink(const char *path)
{
	int res = 0;
	char fullpath[PATH_MAX];

	my_fullpath(fullpath, path);

	res = unlink(fullpath);
	return res;
}

// update the access/modifiy time of one file
static int my_utimes(const char *path, struct utimebuf *ubuffer)
{
	int res = 0;
	char fullpath[PATH_MAX];

	my_fullpath(fullpath, path);

	res = utime(fullpath, ubuffer);
	return res;
}

// write data to one open file
static int my_write(const char *path, const char *buf, size_t size, off_t offset, 
	struct fuse_file_info *file)
{
	int res = 0;
	res = pwrite(file->fh, buf, size, offset);

	return res;
}

static struct fuse_operations my_oper = {
	.access = my_access,
	.chmod = my_chmod,
	.chown = my_chown,
	.create = my_create,
	.getattr = my_getattr,
	.mkdir = my_mkdir,
	.open = my_open,
	.readdir = my_readdir,
	.release = my_release,
	.rmdir = my_rmdir,
	.unlink = my_unlink,
	.utime = my_utimes,
	.write = my_write
};

int main(int argc, char *argv[])
{
/*	if (argc < 3){
		printf("wrong usage, root and mount directories needed");
		return 0;
	}
	*/
	root = realpath(argv[argc-2], NULL);
	argv[argc-2] = argv[argc-1];
	argv[argc-1] = NULL;
	argc--;

	return fuse_main(argc, argv, &my_oper, NULL);
}
