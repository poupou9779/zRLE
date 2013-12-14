#ifndef RLE_H
#define RLE_H

#include <stdio.h>

typedef char BYTE;

#define RLE_OK                 0
#define RLE_FAILED_ALLOCATION -1
#define RLE_NULL_POINTER_ARG  -2

int encode_RLE(const void *, size_t, unsigned int, void **, unsigned int *, int (*)(const void *, const void *));
int decode_RLE(const void *, size_t, unsigned int, void **, unsigned int *);

#endif
