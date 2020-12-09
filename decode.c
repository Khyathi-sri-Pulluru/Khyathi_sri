/*
	Title		:	decode_functions
	Author		:	Khyathisri P
	Organisation	:	Emertxe
	Description	:	All the function definitions are defined here	
*/

#include "main.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint decode_get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* Check CLA
 * Input: Command Line Arguments
 * Output: Status whether the CLA are in required format
 * Description: Checking the format of args
 * <./a.out> <-d> <.bmp> [output.txt]
 */

Status read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo)
{
	char *p;
	int i;
	
		
	if (argv[2] != NULL)
	{
		p = argv[2];
		while (*p++ != '.');
		if (*(p) == 'b' && *(p + 1) == 'm' && (*(p + 2) == 'p'))
		{
			return e_success;
		}
		return e_failure;
			
	}
	if (argv[3] != NULL)
	{
		p = argv[3];
		while (*p++ != '.');
		if (*(p) == 't' && *(p + 1) == 'x' && (*(p + 2) == 't'))
		{
			return e_success;
		}
		return e_failure;
			
	}
}

/* Decoding file_size from the encoded input
 * Input: Stego_img file[Encoded]
 * Output: Decoded value from the image which is file size of secret file
 * Description: Extracting the file_size from encoded image
 */
long decode_get_file_size(FILE *fptr)
{
	fseek(fptr, 0, SEEK_END);
	long file_size = ftell(fptr);
	rewind(fptr);
	printf("size = %ld", file_size);
	return file_size;
}

/* Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status decode_open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    //Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	printf("in src_image1");
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_decoded_secret = fopen(encInfo->decoded_secret_fname, "w");
    //Do Error handling
    if (encInfo->fptr_decoded_secret == NULL)
    {
    	printf("in src_image2");
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->decoded_secret_fname);
    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


Status decode_magic_string(EncodeInfo *encInfo)
{
	char c;
	printf("Magic_string: ");
	c = decode_data_from_image(8, encInfo->fptr_src_image);
	printf("%c", c);
	if (c != '#' && c != '*')
	{
		return e_failure;
	}
	c = decode_data_from_image(8, encInfo->fptr_src_image);
	printf("%c", c);
	if (c != '#' && c != '*')
	{
		return e_failure;
	}
	printf("\n");
	return e_success;
}

char decode_data_from_image(int size, FILE *fptr_src_image)
{
	char buff[size];
	char new = 0;
	fread(buff, size, 1, fptr_src_image);
	for (int i = 0; i < size; i++)
	{
		if (i !=  size - 1)
			new = (new | (buff[i] & 1)) << 1;
		else
			new = new | (buff[i] & 1);
		
	}

	return new;
	
}

void decode_file_extension_size(EncodeInfo *encInfo)
{
	int decode_extn_size;
	decode_extn_size = decode_data_from_image(8 * sizeof(int), encInfo->fptr_src_image);
	printf("decode_extn_size: %d\n", decode_extn_size);
}

void  decode_secret_file_extn(EncodeInfo *encInfo)
{
	char decode_extn;
	printf("file_extn: ");
	for (int i = 0; i <= sizeof(int) - 1; i++)
	{
		decode_extn = decode_data_from_image(8 , encInfo->fptr_src_image);
		printf("%c", decode_extn);
	}
	printf("\n");
}



int decode_secret_file_size(EncodeInfo *encInfo)
{
	int decode_secret_file_size;
	decode_secret_file_size = decode_data_from_image(8 * sizeof(long), encInfo->fptr_src_image);
	printf("decode_secret_file_size: %d\n", decode_secret_file_size);
	return decode_secret_file_size;
}

void decode_secret_file_data(EncodeInfo *encInfo)
{
	printf("INFO: Decoding secret file size\n");
	int secret_file_size = decode_secret_file_size(encInfo);
	printf("INFO: Done\n");
	char decode_secret_data;
	printf("INFO: Decoding secret file Data\n");
	printf("Secret_Info: ");
	for (int i = 0; i <= secret_file_size - 1; i++)
	{
		decode_secret_data = decode_data_from_image(8 , encInfo->fptr_src_image);
		fputc(decode_secret_data, encInfo -> fptr_decoded_secret);
		printf("%c", decode_secret_data);
	}
	printf("\n");
	fclose(encInfo -> fptr_decoded_secret);
}
