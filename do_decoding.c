#include "main.h"
Status do_decoding(EncodeInfo *encInfo, char *argv[])
{
	Status ms;
	fseek(encInfo -> fptr_src_image, 54, SEEK_SET);
	printf("INFO: Decoding Magic String\n");
	ms = decode_magic_string(encInfo);
	if (ms == e_failure)
	{
		return e_failure;
	}
	printf("INFO: Done\n");
	printf("INFO: Decoding secret file extn size\n");
	decode_file_extension_size(encInfo);
	printf("INFO: Done\n");
	printf("INFO: Decoding secret file extn\n");
	decode_secret_file_extn(encInfo);
	printf("INFO: Done\n");
	decode_secret_file_data(encInfo);
	printf("INFO: Done\n");
	printf("INFO: Info Decoded Successfully..........\n");
	return e_success;
}
