#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char *decoded_secret_fname;
    FILE *fptr_decoded_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    

}EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status encode_read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo, char *argv[]);

/* Get File pointers for i/p and o/p files */
Status encode_open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint encode_get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
long encode_get_file_size(FILE *fptr);

/* Copy bmp image header */
Status encode_copy_bmp_header(FILE * fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
void encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode file extension size*/
void encode_file_extension_size(char *argv[], EncodeInfo *);

/* Encode secret file extenstion */
void encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
void encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
void encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
void encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
void encode_byte_tolsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
void encodecopy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
