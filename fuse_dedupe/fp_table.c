#include <stdio.h>
#include <stdlib.h>
#include "fp_table.h"
#include "log.h"
// fingerprint store
// divided into buckets
fp_bucket fp_table[BUCKET_NUM];

/* for test only */
static unsigned int next_chunk_id = 0;
unsigned int get_chunk_id(){
	unsigned int chunk_idx = next_chunk_id;
	next_chunk_id += 1;
	return chunk_idx;
}
/* test end */

// initialize the fingerprint store
int init_fp_table() {
	int i, ret;

	//fp_table = (fp_bucket *)malloc(BUCKET_NUM * sizeof(fp_bucket));

	for (i = 0; i < BUCKET_NUM; i ++) {
		fp_table[i].hashtable = (htab *)calloc(BUCKET_SIZE, sizeof(htab));

		ret = hcreate_r(BUCKET_SIZE, fp_table[i].hashtable);

		if (ret == 0) {
			fprintf(stderr, "Cannot create hashtable!\n");
			return -1;
		}

		fp_table[i].rec_num = 0;
	}

	return 1;
}

// search fingerprint
// return the pointer to the record
enum search_stat search_fp(unsigned int *fp, fp_record **rec) {
	ENTRY e, *retval;
	unsigned int bucket_idx;
	fp_bucket bucket;
	fp_record *fp_rec;
	htab *ht;

	e.key = (char *)malloc(41 * sizeof(char));

	sprintf(e.key, "%08X%08X%08X%08X%08X",
			fp[0],
			fp[1],
			fp[2],
			fp[3],
			fp[4]);

	// locate the bucket
	bucket_idx = fp[4] % BUCKET_NUM;
	bucket = fp_table[bucket_idx];
	ht = fp_table[bucket_idx].hashtable;

	if (hsearch_r(e, FIND, &retval, ht) == 0) {
		//fprintf(stderr, "Cannot find the fingerprint!\n");
		// add this fingerprint to this bucket
		if (bucket.rec_num < BUCKET_SIZE) {
			// add this record to the hashtable
			fp_rec = (fp_record *)calloc(1, sizeof(fp_record));
			fp_rec->chunk_idx = get_chunk_id();
			fp_rec->ref_count = 1;
			e.data = (void *)fp_rec;

			if(hsearch_r(e, ENTER, &retval, ht) == 0) {
				//fprintf(stderr, "Cannot add record to bucket[%d]\n", bucket_idx);
				return REC_ERROR;
			} else {
				fp_rec = (fp_record *)retval->data;
				fprintf(stderr, "Record Added to Bucket[%d]: [%u, %u]!\n", bucket_idx, fp_rec->chunk_idx, fp_rec->ref_count);
				log_msg(stderr, "Record Added to Bucket[%d]: [%u, %u] [%s]\n", bucket_idx, fp_rec->chunk_idx, fp_rec->ref_count, e.key);

				*rec = (fp_record *)(retval->data);
				//fprintf(stderr, "Record Added: [%u, %u]!\n", (*rec)->chunk_idx, (*rec)->ref_count);

				bucket.rec_num += 1;
				return REC_ADDED;
			}
		} else {
			// TODO: table is full, needs to evict a entry
			// for now, provision enough space for hash table
			return REC_ERROR;
		}
	}	else {
		// record found, return the record
		// TODO: increment reference count
		*rec = (fp_record *)(retval->data);
		return REC_FOUND;
	}
}
