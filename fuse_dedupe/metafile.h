/* metafile.h
* fuse_dedupe project
*
*/

#ifndef META_FILE_H_
#define META_FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

typedef struct meta_data{
	unsigned int fp[5];
	unsigned int chunk_id;
	unsigned int size;
} meta_data;

// index = line num in the file
int meta_read(unsigned int index, unsigned int fd, struct meta_data* );

int meta_write(unsigned int index, unsigned int fd, struct meta_data*);

int meta_del(unsigned int index, unsigned int fd);

#endif
