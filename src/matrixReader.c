#include "matrixReader.h"

void fillAngleMatrix(FILE* filename, int mat[MAT_HEIGHT][MAT_WIDTH]) {
	char temp[1024];
	char **values;

	int row = 0;

	while (fgets(temp, 1024, filename) != NULL) {
		values = stringTokenizer(temp, '\t');
		int l = stringVectorLength(values);
		//Kolla så att längden är lika med width
		if (l != MAT_WIDTH) {
			fprintf(stderr, "Length of matrix file doesnt match\n");
			fprintf(stderr, "File: %d\t Internal: %d", l, MAT_WIDTH);
			exit(EXIT_FAILURE);
		}

		for (int col = 0; col < l; col++) {
			if (strIsNum(values[col])) {
				mat[row][col] = str2num(values[col]);
			}
		}
		row++;

		stringVectorFreeFunc(values);
	}
}

void initiateAngleMatrix(void) {
	FILE *file_theta;
	FILE *file_phi;
	if ((file_theta = fopen("./matlab_code/theta.points", "r")) == NULL) {
		char *error = strerror(errno);
		fprintf(stderr, "Error in fopen when opening file_theta, %s\n", error);
		exit(-1);
	}
	if ((file_phi = fopen("./matlab_code/phi.points", "r")) == NULL) {
		char *error = strerror(errno);
		fprintf(stderr, "Error in fopen when opening file_phi, %s\n", error);
		exit(-1);
	}

	fillAngleMatrix(file_theta, theta);
	fclose(file_theta);
	fillAngleMatrix(file_phi, phi);
	fclose(file_phi);
}

int getTheta(int x, int y) {
	if (x < 0) x = 0;
	if (x > (MAT_WIDTH - 1)) x = MAT_WIDTH - 1;
	if (y < 0) y = 0;
	if (y > (MAT_HEIGHT - 1)) y = MAT_HEIGHT - 1;

	return theta[y][x];
}

int getPhi(int x, int y) {
	if (x < 0) x = 0;
	if (x > (MAT_WIDTH - 1)) x = MAT_WIDTH - 1;
	if (y < 0) y = 0;
	if (y > (MAT_HEIGHT - 1)) y = MAT_HEIGHT - 1;

	return phi[y][x];
}
