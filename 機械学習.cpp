// 機械学習.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>


// convert MNIST file to Image structure

#define PIXEL_SIZE 28*28
#define IMAGE_NUMBERS 60000

typedef struct MNIST_IMAGE_t {
	char Data[PIXEL_SIZE];
}MNIST_IMAGE;

int main()
{
	char fileName[] = "C:\\Users\\jojomonkey\\Downloads\\train-images-idx3-ubyte\\train-images.idx3-ubyte";
	errno_t error;
	FILE *fp;
	// open MNIST image file
	error = fopen_s(&fp, fileName, "rb");
	if (error != NULL) {
		fprintf(stderr, "ERROR: Cannot open %s.\n", fileName);
		return 1;
	}

	{
		// read magic number 0x0000, 2byte
		char magic[2];
		int sz = fread(magic, 1, 2, fp);
		if (sz != 2) {
			printf("Wrong Magic Number.\n");
			return  1;
		}
		if (magic[0] != 0 || magic[1] != 0) {
			printf("Wrong Magic Number.\n");
			return  1;
		}
		printf("Magic Number is 0x0000.\n");
	}

	{
		// read magic datatype, 1byte
		char datatype;
		int sz = fread(&datatype, 1, 1, fp);
		if (sz != 1) {
			printf("Wrong Datatype.\n");
			return  1;
		}
		if (datatype != 0x08) {
			printf("Wrong Datatype.\n");
			return  1;
		}
		printf("Datatype is %d.\n", (int)datatype);
	}

	{
		// read dimensions
		char dim;
		int sz = fread(&dim, 1, 1, fp);
		if (sz != 1) {
			printf("Wrong Dimensions.\n");
			return  1;
		}
		if (dim != 3) {
			printf("Wrong Dimensions.\n");
			return  1;
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
		char *data;
		static MNIST_IMAGE MnistImage[IMAGE_NUMBERS];

		for (i = 0; i < 3; i++) {
			sz = fread(OneDimSize, 1, 4, fp);
			if (sz != 4) {
				printf("Wrong Dimension's Bits.\n");
				return 1;
			}
			DimNumbers[i] = 0;
			for (j = 0; j < 4; j++) {
				DimNumbers[i] = DimNumbers[i] + ((OneDimSize[j] & 0xFF) << 8 * (3 - j));
			}
			printf("Dimension %d is %d.\n", i, DimNumbers[i]);
		}

		totallength = DimNumbers[0] * DimNumbers[1] * DimNumbers[2];
		data = (char *)malloc(totallength);

		if (data == NULL) {
			printf("Malloc miss.\n");
		}
		sz = fread(data, 1, totallength, fp);
		if (sz != totallength) {
			printf("Wrong DataSize.\n");
		}

		//IMAGE_NUMBERS個分のImage構造体に輝度値情報をセット
		for (i = 0; i < IMAGE_NUMBERS; i++) {
			for (j = 0; j < PIXEL_SIZE; j++) {
				MnistImage[i].Data[j] = data[j + i * PIXEL_SIZE];
			}
		}
		free(data);
	}
	fclose(fp);
}
