#include "main.h"

Status do_encoding(EncodeInfo *encInfo, char *argv[])
{
	Status status_copy_header;
	printf("INFO: Copying Image Header\n");
	status_copy_header = encode_copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
	if (status_copy_header == e_failure)
	{
		printf("Invalid: You given Empty input file\nPlease give a valid input\n");
		return e_failure;
	}
	printf("INFO: Done\n");
	char *magic_string = MAGIC_STRING;
	printf("INFO: Encoding Magic String Signature\n");
	encode_magic_string(magic_string, encInfo);
	printf("INFO: Done\n");
	printf("INFO: Encoding %s File Extension\n", encInfo -> secret_fname);
	encode_file_extension_size(argv, encInfo);
	printf("INFO: Done\n");
	long file_size = encode_get_file_size(encInfo -> fptr_secret);
	printf("INFO: Done\n");
	printf("INFO: Encoding %s File Size\n", encInfo -> secret_fname);
	encode_secret_file_size(file_size, encInfo);
	printf("INFO: Done\n");
	printf("INFO: Encoding %s file Data\n", encInfo -> secret_fname);
	encode_secret_file_data(encInfo);
	printf("INFO: Done\n");
	printf("INFO: Copying Left Over Data\n");
	encode_copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
	printf("INFO: Done\n");
	printf("INFO: Info Encoded Successfully..........\n");
	return e_success;
}
