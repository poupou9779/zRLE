#include <string.h> /*strlen*/
#include <stdio.h>  /*printf, fprintf*/
#include <stdlib.h> /*FILE, fopen*/
#include "RLE.h"    /*prototypes*/

/*
    returns RLE_OK if no problem has occured,
            RLE_FAILED_ALLOCATION if the allocation has failed,
            RLE_NULL_POINTER_ARG if any argument is a NULL pointer.
    args :
        - const char *encoded is the string containing the encoded string ;
        - unsigned int length_e is the length of the enoded string ;
        - char **decoded is a pointer on the string that will contain the decoded string ;
        - unsigned int *length_d is a pointer on the length of the decoded string ;
            those 2 last args are modifed by the function.
*/
int decode_RLE(const char *encoded, unsigned int length_e, char **decoded, unsigned int *length_d)
{
    unsigned int i,
                 j,
                 index_d = 0;
    *length_d = 0;

    if(encoded == NULL)
        fprintf(stderr, "Error decode_RLE : No encoded string sent (decode)\n");
    else if(decoded == NULL)
        fprintf(stderr, "Error decode_RLE : No free space to decode (decode)\n");
    else if(length_d == NULL)
        fprintf(stderr, "Error decode_RLE : NULL-pointer sent for the length (decode)\n");
    else
    {
        for(j = 0; j < length_e && encoded[j] != '\0'; j += 2)
        {
            (*length_d) += (unsigned int)encoded[j];
        }

        *decoded = malloc((*length_d+1) * sizeof(char));

        if(*decoded == NULL)
        {
            fprintf(stderr, "Error decode_RLE : Failed to allocate the string to decode\n");
            return RLE_FAILED_ALLOCATION;
        }
        else
        {
            for(j = 0; j < length_e && encoded[j] != '\0'; j += 2)
                for(i = 0; i < (unsigned int)encoded[j]; ++i)
                    (*decoded)[index_d++] = encoded[j+1];
            (*decoded)[*length_d] = '\0';
        }
        return RLE_OK;
    }
    return RLE_NULL_POINTER_ARG;
}

/*
    returns RLE_OK if no problem has occured,
            RLE_FAILED_ALLOCATION if the allocation has failed,
            RLE_NULL_POINTER_ARG if any argument is a NULL pointer.
    args :
        - const char *initial is the string containing the not encoded yet string ;
        - unsigned itn length_i is the length of the not encoded yet string ;
        - char **encoded is a pointer on the string that will contain the encoded string ;
        - unsigned int *length_e is a pointer on the length of the encoded string.
*/
int encode_RLE(const char *initial, unsigned int length_i, char **encoded, unsigned int *length_e)
{
    char tmp_char;
    unsigned int i,
                 count,
                 index = 0;

    if(initial == NULL)
        fprintf(stderr, "Error encode_RLE : No string to encode was sent (encode)\n");
    else if(encoded == NULL)
        fprintf(stderr, "Error encode_RLE : No string sent to encode the string (encode)\n");
    else if(length_e == NULL)
        fprintf(stderr, "Error encode_RLE : NULL-pointer was sent for the length (encode)\n");
    else
    {
        *length_e = 0;
        for(count = 0, tmp_char = *initial, i = 0; i <= length_i; ++i)
        {
            if(initial[i] != tmp_char || count == 255)
            {
                *length_e += 2;
                count = 1;
                tmp_char = initial[i];
            }
            else
                ++count;
        }

        *encoded = malloc((*length_e+1) * sizeof(char));

        if(*encoded == NULL)
        {
            fprintf(stderr, "Error encode_RLE : No memory allocated to encode\n");
            return RLE_FAILED_ALLOCATION;
        }
        else
        {
            for(count = 0, tmp_char = *initial, i = 0; i <= length_i; ++i)
            {
                if(initial[i] != tmp_char || count == 255)
                {
                    (*encoded)[index++] = (char)count;
                    (*encoded)[index++] = tmp_char;
                    count = 1;
                    tmp_char = initial[i];
                }
                else
                    ++count;
            }
            (*encoded)[*length_e] = '\0';
        }
        return RLE_OK;
    }
    return RLE_NULL_POINTER_ARG;
}
/*
    returns RLE_OK if no problem has occured,
            RLE_NULL_POINTER_ARG if any of the args is a NULL pointer
            the error value returned by encode_RLE() if an error has occured;
    args :
        - FILE *initial_file is the pre-opened file to encode
        - FILE *encoded_file is the pre-opened file which will contain the encoded content of initial_file
*/
int encode_file_RLE(FILE *initial_file, FILE *encoded_file)
{
    char buffer[LENGTH],
        *tmp_str;
    unsigned int length_e;
    int ret_value;
    if(initial_file == NULL)
    {
        fprintf(stderr, "Error encode_file_RLE : initial file is a NULL pointer\n");
        return RLE_NULL_POINTER_ARG;
    }
    else if(encoded_file == NULL)
    {
        fprintf(stderr, "Error encode_file_RLE : encoded_file is a NULL pointer\n");
        return RLE_NULL_POINTER_ARG;
    }
    while(fgets(buffer, (LENGTH&1) ? LENGTH : LENGTH-1, initial_file) != NULL)
    {
        if((ret_value = encode_RLE(buffer, strlen(buffer), &tmp_str, &length_e)) != RLE_OK)
        {
            fprintf(stderr, "Error encode_file_RLE : Unable to encode : %s\n", buffer);
            if(ret_value == RLE_FAILED_ALLOCATION)
                fprintf(stderr, "\tallocation has failed\n");
            else if(ret_value == RLE_NULL_POINTER_ARG)
                fprintf(stderr, "\tnull pointer was sent\n");
            return ret_value;
        }
        fprintf(encoded_file, tmp_str);
        free(tmp_str);
        tmp_str = NULL;
    }
    return RLE_OK;
}

/*
    returns RLE_OK if no problem has accured,
            RLE_NULL_POINTER_ARG if any of the args is a NULL pointer
            the error value returned by decode_RLE() if an error has occured
    args :
        - FILE *encoded_file is the pre-opened encoded file to decode
        - FILE *decoded_file is the pre-opened file which will contain the decoded content of encoded_file
*/
int decode_file_RLE(FILE *encoded_file, FILE *decoded_file)
{
    char buffer[LENGTH],
        *tmp_str;
    unsigned int length_d;
    if(encoded_file == NULL)
    {
        fprintf(stderr, "Error decode_file_RLE : encoded_file is a NULL pointer\n");
        return RLE_NULL_POINTER_ARG;
    }
    else if(decoded_file == NULL)
    {
        fprintf(stderr, "Error decode_file_RLE : decoded_file is a NULL pointer\n");
        return RLE_NULL_POINTER_ARG;
    }
    while(fgets(buffer, (LENGTH&1) ? LENGTH : LENGTH-1, encoded_file) != NULL)
    {
        if(decode_RLE(buffer, strlen(buffer), &tmp_str, &length_d) == -1)
        {
            printf("Error decode_file_RLE : Unable to decompress : %s\n", buffer);
            return -1;
        }
        fprintf(decoded_file, tmp_str);
        free(tmp_str);
    }
    return 0;
}
