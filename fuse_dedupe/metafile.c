/* metafile.c
* fuse_dedupe project
*
*/

#include "metafile.h"
#include "log.h"

#include <unistd.h>

// read the struct information from the meta file,according to the 
int meta_read(unsigned int index, unsigned int fd, struct meta_data *metadata)
{
	lseek(fd, index*sizeof(struct meta_data), SEEK_SET);
	int res;
	res = read(fd, metadata, sizeof(struct meta_data));
	/*if (res == EOF)
		return 0;
	else
		return 1;
		*/
	return res;
}


int meta_write(unsigned int index, unsigned int fd, struct meta_data *metadata)
{
	lseek(fd, index*sizeof(struct meta_data), SEEK_SET);
	int res=0;
	res = write(fd, metadata, sizeof(struct meta_data));
	if (res == -1)
		log_msg("\nmeta data write failed for %d at %d\n", fd, index);

	return res;
}

int meta_del(unsigned int index, unsigned int fd)
{
	lseek(fd, index*sizeof(struct meta_data), SEEK_SET);
	int res=0;
	res = write(fd, EOF, sizeof(int));
	if (res == -1)
		log_msg("\nmeta data write failed for %d at %d\n", fd, index);

	return res;
}
