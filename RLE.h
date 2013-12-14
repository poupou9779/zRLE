#ifndef RLE_H
#define RLE_H

#define LENGTH 255

#define RLE_OK                 0
#define RLE_FAILED_ALLOCATION -1
#define RLE_NULL_POINTER_ARG  -2

int encode_RLE(const char *, unsigned int, char **, unsigned int *);
int decode_RLE(const char *, unsigned int, char **, unsigned int *);
int encode_file_RLE(FILE *, FILE *);
int decode_file_RLE(FILE *, FILE *);

#endif
