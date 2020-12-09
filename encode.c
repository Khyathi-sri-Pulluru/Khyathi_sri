#include "main.h"
/* Function Definitions */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	char *p;
	int i;
	for (i = 2; i <= 3; i++)
	{
		
		if (argv[2] != NULL)
		{
			p = argv[2];
			while (*p++ != '.');
			if (*p == 'b' && *(p + 1) == 'm' && *(p + 2) == 'p')
			{
				return e_success;
			}
			return e_failure;
				
		}
		if (argv[3] != NULL)
		{
			p = argv[3];
			while (*p++ != '.');
			if (*p == 't' && *(p + 1) == 'x' && *(p + 2) == 't')
			{
				return e_success;
			}
			return e_failure;	
		}
		
	}

}
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint encode_get_image_size_for_bmp(FILE *fptr_image)
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


long encode_get_file_size(FILE *fptr)
{
	fseek(fptr, 0, SEEK_END);
	long file_size = ftell(fptr);
	rewind(fptr);
	printf("size = %ld\n", file_size);
	return file_size;
	//encode_secret_file_size(file_size, &encInfo);
}

/* Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status encode_open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    //Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    //Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    //Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
	if (encode_get_image_size_for_bmp(encInfo -> fptr_src_image) > encode_get_file_size(encInfo -> fptr_secret))
	{
		printf ("INFO: Capacity checked\n");
		return e_success;
	}
	else
	{
		printf ("INFO: Capacity Exceeded\n");
		return e_failure;
	}
}

Status encode_copy_bmp_header(FILE * fptr_src_image, FILE *fptr_dest_image)
{
	//Declare variables here
	char buff[54];
	
	if (fread (buff, 54, 1, fptr_src_image) > 0 && fwrite(buff, 54, 1, fptr_dest_image) > 0)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}		
}

void encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
	char c;
	c = magic_string[0];
	encode_data_to_image(&c, 8, encInfo->fptr_src_image, encInfo->fptr_stego_image);
	c = magic_string[1];
	encode_data_to_image(&c, 8, encInfo->fptr_src_image, encInfo->fptr_stego_image);
	
}

void encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	char buff[size], c;
	fread(buff, size, 1, fptr_src_image);
	for (int i = 0; i < MAX_IMAGE_BUF_SIZE; i++)
	{
		c = (*data >> (MAX_IMAGE_BUF_SIZE - (i + 1)) & 1) ;
		encode_byte_tolsb(c, buff + i);
	}
	
	fwrite(buff, size, 1, fptr_stego_image);

}

void encode_byte_tolsb(char data, char *image_buffer)
{
	data = data & 1;
	*image_buffer = (*image_buffer & (~0 << 1)) ;
	*image_buffer = *image_buffer | data;
}

void encode_file_extension_size(char *argv[], EncodeInfo *encInfo)
{
	char *p = argv[3];
	char *q;
	int count = 0;
	while (*p != '.')
	{
		p++;
	}

	q = p;
	while (*q)
	{
		count++;
		q++;
	}
	
	q = (char *)&count;
	
	for (int i = sizeof(int) - 1; i >= 0; i--)
	{
	
		encode_data_to_image(q + i, 8, encInfo->fptr_src_image, encInfo->fptr_stego_image);
	}
	printf("INFO: Done\n");
	printf("INFO: Encoding %s file Extension\n", encInfo -> secret_fname);
	encode_secret_file_extn(p, encInfo);
	
		

}


void encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	printf("file_extn = %s", file_extn);
	
	while (*file_extn)
	{
		encode_data_to_image(file_extn, 8, encInfo->fptr_src_image, encInfo->fptr_stego_image);
		file_extn++;
	}
	
	
}

void encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
	char *q;
	q = (char *)&file_size;
	
	for (int i = sizeof(long) - 1; i >= 0; i--)
	{
		encode_data_to_image(q + i, 8, encInfo->fptr_src_image, encInfo->fptr_stego_image);
	}
	
}

void encode_secret_file_data(EncodeInfo *encInfo)
{
	char ch;
	printf("In secret file data: ");
	while ((ch = fgetc(encInfo -> fptr_secret) )!= EOF)
	{
		printf("%c", ch);
		encode_data_to_image(&ch, 8, encInfo->fptr_src_image, encInfo->fptr_stego_image);
	}
	printf("\n");
}

void encode_copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	char buff[10000];
	while (fread(buff, 100, 100, fptr_src) > 0)
	{
		fwrite(buff, 100, 100, fptr_dest);
	}
}
