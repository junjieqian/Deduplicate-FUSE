#ifndef CHUNK_STORE_H_
#define CHUNK_STORE_H_

#include "dedupe.h"

#define MAX_CHUNKS_PER_FILE 8192
#define BUF_SIZE 256;

// store the current fd, to avoid frequently open the file
static int store_fd = -1;

// initialize the path to chunk store director
int init_chunk_store(const char *path);

// read a chunk with index chunk_idx
int read_chunk(unsigned int chunk_idx, char* buf);

// write_chunk to index chunk_idx
int write_chunk(unsigned int chunk_idx, const char *buf);

int close_chunk_store();
#endif
