#ifndef FP_TABLE_H_
#define FP_TABLE_H_

#define __USE_GNU
#include <search.h>

#define BUCKET_NUM 1024
#define BUCKET_SIZE 65536

// fingerprint store which contains 
typedef struct hsearch_data htab;

// Structure of the record in a fingerprint table
typedef struct fp_record {
  unsigned int chunk_idx;
	unsigned int ref_count;
} fp_record;

typedef struct fp_bucket {
	htab *hashtable;
	unsigned int rec_num;
} fp_bucket;

enum search_stat {
	REC_FOUND,
	REC_ADDED,
	REC_ERROR
};

int init_fp_table();
enum search_stat search_fp(unsigned int *fp, fp_record **rec);

#endif
