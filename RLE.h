#ifndef RLE_H
#define RLE_H

#include <stdio.h>

typedef char BYTE;

#define LENGTH  255

#define RLE_OK                 0
#define RLE_FAILED_ALLOCATION -1
#define RLE_NULL_POINTER_ARG  -2
#define RLE_WRITING_ERROR     -3

int encode_RLE(const void *, size_t, unsigned int, void **, unsigned int *, int (*)(const void *, const void *));
int decode_RLE(const void *, size_t, unsigned int, void **, unsigned int *);

int encode_file_RLE(FILE *, FILE *, size_t, int (*)(const void *, const void *));
int decode_file_RLE(FILE *, FILE *, size_t);

#endif
