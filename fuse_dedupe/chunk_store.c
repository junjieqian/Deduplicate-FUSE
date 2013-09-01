#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "chunk_store.h"

int init_chunk_store(const char *path) {
	store_fd = open(path, O_RDWR | O_APPEND | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP);
	if (store_fd < 0) {
		fprintf(stderr, "Failed to initialize chunk store!\n");
		return -1;
	} else {
		return 1;
	}
}

int close_chunk_store() {
	close(store_fd);
}

int read_chunk(unsigned int chunk_idx, char *buf) {
	unsigned int offset;
	int ret;

	offset = chunk_idx * CHUNK_SIZE;
	
	if (store_fd < 0) {
		fprintf(stderr, "Chunk store not initilized!\n");
		return -1;
	}
	
	lseek(store_fd, offset, SEEK_SET);

	ret = read(store_fd, buf, CHUNK_SIZE);

	if (ret != CHUNK_SIZE) {
		fprintf(stderr, "Error in reading file!\n");
		return -1;
	}

	return 1;
}

int write_chunk(unsigned int chunk_idx, const char *buf) {
	unsigned int offset;
	int ret;

	offset = chunk_idx * CHUNK_SIZE;
	
	if (store_fd < 0) {
		fprintf(stderr, "Chunk store not initilized!\n");
		return -1;
	}
	
	lseek(store_fd, offset, SEEK_SET);

	ret = write(store_fd, buf, CHUNK_SIZE);

	if (ret != CHUNK_SIZE) {
		fprintf(stderr, "Error in reading file!\n");
		return -1;
	}

	return 1;
}
