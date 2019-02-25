
#include "stdafx.h"
#include "MNISTtype.h"
#include <stdio.h>
#include <stdlib.h>

bool SetImageData(MNIST_IMAGE *MnistImage, char *fileName1) {
	int loopnumbers;
	errno_t error;
	FILE *fp1;
	// open MNIST image file
	error = fopen_s(&fp1, fileName1, "rb");
	if (error != NULL) {
		fprintf(stderr, "ERROR: Cannot open %s.\n", fileName1);
		return FALSE;
	}

	{
		// read magic number 0x0000, 2byte
		char magic[2];
		int sz = fread(magic, 1, 2, fp1);
		if (sz != 2) {
			printf("Wrong Magic Number.\n");
			return  FALSE;
		}
		if (magic[0] != 0 || magic[1] != 0) {
			printf("Wrong Magic Number.\n");
			return  FALSE;
		}
		printf("Magic Number is 0x0000.\n");
	}

	{
		// read magic datatype, 1byte
		char datatype;
		int sz = fread(&datatype, 1, 1, fp1);
		if (sz != 1) {
			printf("Wrong Datatype.\n");
			return  FALSE;
		}
		if (datatype != 0x08) {
			printf("Wrong Datatype.\n");
			return  FALSE;
		}
		printf("Datatype is %d.\n", (int)datatype);
	}

	{
		// read dimensions
		char dim;
		int sz = fread(&dim, 1, 1, fp1);
		if (sz != 1) {
			printf("Wrong Dimensions.\n");
			return  FALSE;
		}
		if (dim != 3) {
			printf("Wrong Dimensions.\n");
			return  FALSE;
		}
		printf("Dimension is %d.\n", (int)dim);
	}

	{
		// each dimension's size
		int DimNumbers[3];
		char OneDimSize[4];
		int i, j;
		int sz;
		int totallength;
		unsigned char *data;

		for (i = 0; i < 3; i++) {
			sz = fread(OneDimSize, 1, 4, fp1);
			if (sz != 4) {
				printf("Wrong Dimension's Bits.\n");
				return FALSE;
			}
			DimNumbers[i] = 0;
			for (j = 0; j < 4; j++) {
				DimNumbers[i] = DimNumbers[i] + ((OneDimSize[j] & 0xFF) << 8 * (3 - j));
			}
			printf("Dimension %d is %d.\n", i, DimNumbers[i]);
		}

		totallength = DimNumbers[0] * DimNumbers[1] * DimNumbers[2];
		loopnumbers = DimNumbers[0];
		data = (unsigned char *)malloc(totallength);

		if (data == NULL) {
			printf("Malloc miss.\n");
		}
		sz = fread(data, 1, totallength, fp1);
		if (sz != totallength) {
			printf("Wrong DataSize.\n");
		}

		//IMAGE_NUMBERS個分のImage構造体に輝度値情報をセット
		for (i = 0; i < loopnumbers; i++) {
			for (j = 0; j < PIXEL_SIZE; j++) {
				MnistImage[i].Data[j] = ((double)data[j + i * PIXEL_SIZE]) / 255;
			}
		}
		free(data);
	}
	fclose(fp1);

	return TRUE;
}

bool SetLabelData(MNIST_IMAGE *MnistImage, char *fileName1) {
	int loopnumbers;
	errno_t error;
	FILE *fp1;

	// open MNIST label file
	error = fopen_s(&fp1, fileName1, "rb");
	if (error != NULL) {
		fprintf(stderr, "ERROR: Cannot open %s.\n", fileName1);
		return FALSE;
	}

	{
		// read magic number 0x0000, 2byte
		char magic[2];
		int sz = fread(magic, 1, 2, fp1);
		if (sz != 2) {
			printf("Wrong Magic Number.\n");
			return  FALSE;
		}
		if (magic[0] != 0 || magic[1] != 0) {
			printf("Wrong Magic Number.\n");
			return  FALSE;
		}
		printf("Magic Number is 0x0000.\n");
	}

	{
		// read magic datatype, 1byte
		char datatype;
		int sz = fread(&datatype, 1, 1, fp1);
		if (sz != 1) {
			printf("Wrong Datatype.\n");
			return  FALSE;
		}
		if (datatype != 0x08) {
			printf("Wrong Datatype.\n");
			return  FALSE;
		}
		printf("Datatype is %d.\n", (int)datatype);
	}

	{
		// read dimensions
		char dim;
		int sz = fread(&dim, 1, 1, fp1);
		if (sz != 1) {
			printf("Wrong Dimensions.\n");
			return  FALSE;
		}
		if (dim != 1) {
			printf("Wrong Dimensions.\n");
			return  FALSE;
		}
		printf("Dimension is %d.\n", (int)dim);
	}

	{
		// each dimension's size
		int DimNumbers[1];
		char OneDimSize[4];
		int i, j;
		int sz;
		int totallength;
		unsigned char *data;

		for (i = 0; i < 1; i++) {
			sz = fread(OneDimSize, 1, 4, fp1);
			if (sz != 4) {
				printf("Wrong Dimension's Bits.\n");
				return FALSE;
			}
			DimNumbers[i] = 0;
			for (j = 0; j < 4; j++) {
				DimNumbers[i] = DimNumbers[i] + ((OneDimSize[j] & 0xFF) << 8 * (3 - j));
			}
			printf("Dimension %d is %d.\n", i, DimNumbers[i]);
		}

		totallength = DimNumbers[0];
		loopnumbers = DimNumbers[0];
		data = (unsigned char *)malloc(totallength);

		if (data == NULL) {
			printf("Malloc miss.\n");
		}
		sz = fread(data, 1, totallength, fp1);
		if (sz != totallength) {
			printf("Wrong DataSize.\n");
		}

		for (i = 0; i < loopnumbers; i++) {
			MnistImage[i].idealLabel = data[i];
			for (j = 0; j < OUTPUT_NUMBERS; j++) {
				MnistImage[i].Label[j] = ((int)data[i] == j ? 1 : 0);
			}
		}
		free(data);
	}
	fclose(fp1);
	return TRUE;
}