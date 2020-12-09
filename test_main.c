/*
	Encoding and Decoding image data by manipulating lsb bit of each byte.
*/

#include "main.h"

int main(int argc, char *argv[])
{
	EncodeInfo encInfo;
	uint img_size;
	char buff[54];
	OperationType operation_type;
	Status validate_args;
	uint file_size;

	operation_type = check_operation_type(argv);
	validate_args = read_and_validate_encode_args(argv, &encInfo);
	if (operation_type == e_encode)
	{	
		if (validate_args == e_success)
		{
			printf("INFO: ##Encoding Procedure Started##\n");

			// Fill with sample filenames
			encInfo.src_image_fname = argv[2];
			encInfo.secret_fname = argv[3];
			if (argv[4] == NULL)
			{
				encInfo.stego_image_fname = "stego_img.bmp";
				printf("Output file not mentioned. Creating %s as default output file\n", encInfo.stego_image_fname);
			}
			else
			{
				encInfo.stego_image_fname = argv[4];
			}


			// Test open_files
			if (encode_open_files(&encInfo) == e_failure)
			{
				printf("INFO: Unable to Open required files\n");
				return 1;
			}
			else
			{
				printf("INFO: Opened required files\n");
			}

			// Test get_image_size_for_bmp
			img_size = encode_get_image_size_for_bmp(encInfo.fptr_src_image);
			printf("INFO: Image size = %u\n", img_size);

			encInfo.fptr_src_image = fopen(encInfo.src_image_fname, "rb");

			//Handle the error
			if (!encInfo.fptr_src_image)
			{
				perror("fopen argv[2]");
				exit(1);
			}
			else
			{
				printf("INFO: Opened %s\n", encInfo.src_image_fname);
			}

			encInfo.fptr_secret = fopen(encInfo.secret_fname, "rb");

			//Handle the error
			if (!encInfo.fptr_secret)
			{
				perror("fopen argv[3]");
				exit(1);
			}
			else
			{
				printf("INFO: Opened %s\n", encInfo.secret_fname);
			}

			encInfo.fptr_stego_image = fopen(encInfo.stego_image_fname, "wb");
			if (!encInfo.fptr_stego_image)
			{
				perror("fopen argv[4]");
				exit(1);
			}
			else
			{
				printf("INFO: Opened %s\n", encInfo.stego_image_fname);
			}
			printf("INFO: Done\n");

			if (do_encoding(&encInfo, argv) == e_failure)
			{
				printf("INFO: Input file is Empty\n");
				return 255;
			}
		}
		else
		{
			printf("<./a.out> <-e> <.bmp> <.txt> [output.bmp]\n");
			return 255;
		}
	}
	else if(operation_type == e_decode)
	{
		if (validate_args == e_success)
		{
			printf("INFO: ##Decoding Procedure Started##\n");
			encInfo.src_image_fname = argv[2];
			if (argv[3] == NULL)
			{
				encInfo.decoded_secret_fname = "decode.txt";
				printf("Output file not mentioned. Creating %s as default output file\n", encInfo.decoded_secret_fname);
			}
			else
			{
				encInfo.decoded_secret_fname = argv[3];
			}


			//encInfo.stego_image_fname = "beautiful_decode.bmp";

			// Test open_files
			if (decode_open_files(&encInfo) == e_failure)
			{
				printf("INFO: Unable to open required files\n");
				return 255;
			}
			else
			{
				printf("INFO: Opened required files\n");
			}

			// Test get_image_size_for_bmp
			img_size = decode_get_image_size_for_bmp(encInfo.fptr_src_image);
			printf("INFO: Image size = %u\n", img_size);

			encInfo.fptr_src_image = fopen(encInfo.src_image_fname, "rb");

			//Handle the error
			if (!encInfo.fptr_src_image)
			{
				perror("fopen argv[2]");
				exit(1);
			}
			printf("INFO: Opened %s\n", encInfo.src_image_fname);

			printf("INFO: Done\n");

			encInfo.fptr_secret = fopen(encInfo.decoded_secret_fname, "w");

			//Handle the error
			if (!encInfo.fptr_secret)
			{
				perror("fopen argv[3]");
				exit(1);
			}
			printf("INFO: Opened %s\n", encInfo.decoded_secret_fname);

			printf("INFO: Done\n");

			if (do_decoding(&encInfo, argv) == e_failure)
			{
				printf("\nINFO: Input file---> Empty or give a decoded image file\n");
				return 255;
			}
			// do_decoding(&encInfo, argv);

		}
		else
		{
			printf("<./a.out> <-d> <.bmp> [output.txt]");
			return 255;
		}
	}

	else
	{
		printf("<./a.out> <-e or -d> <.bmp> <.txt> [output.txt]");
		return 255;
	}

}

OperationType check_operation_type(char *argv[])
{
	char *p = argv[1];

	if (strcmp(p, "-e") == 0)
		return e_encode;
	else if (strcmp(p, "-d")==0)
		return e_decode;
	else
		return e_unsupported;
}
