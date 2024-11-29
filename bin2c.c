#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
	const char* filename = strrchr(filename, '\\') + 1;

	if (argc < 4)
	{
		fprintf(stderr, "Usage: %s <input file path> <output file path> <array name>\n", filename);
		return 1;
	}

	FILE* fInput;
	fopen_s(&fInput, argv[1], "rb");

	if (!fInput)
	{
		fprintf(stderr, "%s: can't open %s for reading\n", filename, argv[1]);
		return 2;
	}

	fseek(fInput, 0, SEEK_END);
	const long long fileSize = ftell(fInput);
	fseek(fInput, 0, SEEK_SET);

	unsigned char* buffer = (unsigned char*)malloc(fileSize);
	if (!buffer)
	{
		fprintf(stderr, "%s: failed to allocate memory\n", filename);
		fclose(fInput);
		return 3;
	}

	fread(buffer, fileSize, 1, fInput);
	fclose(fInput);

	FILE* fOutput;
	fopen_s(&fOutput, argv[2], "w");

	if (!fOutput)
	{
		fprintf(stderr, "%s: can't open %s for writing\n", filename, argv[2]);
		free(buffer);
		return 4;
	}

	const char* arrayName = argv[3];

	fprintf(fOutput, "long long %s_length = %i;\n", arrayName, fileSize);
	fprintf(fOutput, "unsigned char %s[] =\n{", arrayName);

	for (long long i = 0; i < fileSize; ++i)
	{
		if (i % 16 == 0)
			fprintf(fOutput, "\b\n\t");
		fprintf(fOutput, "0x%02X, ", buffer[i] & 0xFF);
	}

	fprintf(fOutput, "\b\b\n};");
	fclose(fOutput);
	free(buffer);
}
