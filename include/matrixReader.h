#ifndef MATRIXREADER_H
#define MATRIXREADER_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "myStringLib.h"

#define MAT_WIDTH 26
#define MAT_HEIGHT 36

static int theta[MAT_HEIGHT][MAT_WIDTH];
static int phi[MAT_HEIGHT][MAT_WIDTH];

void fillAngleMatrix(FILE* filenamn, int mat[MAT_HEIGHT][MAT_WIDTH]);
void initiateAngleMatrix(void);
int getTheta(int x, int y);
int getPhi(int x, int y);
#endif
