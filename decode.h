#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include "encode.h"

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status decode_read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the decoding */
Status do_decoding(EncodeInfo *encInfo, char *argv[]);

/* Get File pointers for i/p and o/p files */
Status decode_open_files(EncodeInfo *encInfo);

/* Store Magic String */
Status decode_magic_string(EncodeInfo *encInfo);

/*Decoding file_extn_size*/
void decode_file_extension_size(EncodeInfo *);

/* Decode secret file extenstion */
void decode_secret_file_extn(EncodeInfo *encInfo);

/* Decode secret file size */
int decode_secret_file_size(EncodeInfo *encInfo);

/* Decode secret file data*/
void decode_secret_file_data(EncodeInfo *encInfo);

/* Decode function, which does the real decoding */
char decode_data_from_image(int size, FILE *fptr_src_image);

#endif
