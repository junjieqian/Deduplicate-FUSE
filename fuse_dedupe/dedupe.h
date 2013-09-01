/* dedupe.h
*
*
*/

#ifndef __DEDUPE_H
#define __DEDUPE_H

#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNK_SIZE 4096

struct table{
char* key;
long value;
};

// generate the sha1 value of the file blocks
char sha1(char data[]);

// find the match out in the table
// if successful the match value returned, else -1 returned
long findmatch(table pagetable, char* in);

// find the available block address
long findblank();

// insert the sha1 value to the table
void insert(table pagetable, char* in, long out);

#endif
