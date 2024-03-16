#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "tinyexpr.h"
//H0;W1
#define HZ ceil(dimensions[0]/2.0)
#define WZ ceil(dimensions[1]/2.0)

char bg=' ';
char line='#';

void boardInit(int dimensions[], char grid[dimensions[0]][dimensions[1]]){
	for (int i=0; i<=dimensions[0]-1; i++) {
		for (int j=0; j<=dimensions[1]-1; j++) {
			grid[i][j]=bg;
		}
	}
	for (int j=0; j<=dimensions[1]-1; j++) {
		int a = HZ;
		grid[a][j]='_';
	}
	for (int i=0; i<=dimensions[0]-1; i++) {
		int a = WZ;
		grid[i][a]='|';
	}
}

int function( double x, double scale[], char *argv[]){
	int err;
	te_variable vars[] = {"x", &x};
	te_expr *n = te_compile(argv[1], vars, 1, &err);
	x = scale[1]*x;
	x = scale[0]*te_eval(n);
	return round(x);
}

void funcExec( int dimensions[], double scale[] , char grid[dimensions[0]][dimensions[1]], char *argv[]){
	for (double j = dimensions[1] - 1; j >= 0; j=j-0.01) {
		int row = HZ - function(j - WZ, scale, argv);
		if (row >= 0 && row < dimensions[0] && j >= 0 && j < dimensions[1]) {
			int b = round(j);
			grid[row][b] = line;
		}
	}
}

void boardRender( int dimensions[2], char grid[dimensions[0]][dimensions[1]]){
	for (int i=0; i<=dimensions[0]-1; i++) {
		for (int j=0; j<=dimensions[1]-1; j++) {
			printf("%c", grid[i][j]);
		}
		printf("\n");
	}
}

void paramChecker( int argc, char *argv[], int dimensions[], double scale[]){
	errno = 0;
	int defDimensions[]={100,50};
	double defScale[]={10,0.1};
	if (argc < 5){
		perror("Not enough arguments, defaulting to defaults");
		for (int i = 0; i<2; i++) {
			dimensions[i] = defDimensions[1-i];
		}
		for (int i = 0; i<2; i++) {
			scale[i] = defScale[i];
		}
	}else{
		for (int i = 0; i<2; i++) {
			char *ptr = NULL;
			dimensions[i] = strtod(argv[3-i], &ptr);
			if (errno != 0 || *ptr != '\0'){
				dimensions[i] = defDimensions[1-i];
			}
		}
		for (int i = 0; i<2; i++) {
			char *ptr = NULL;
			scale[i] = strtod(argv[i+4], &ptr);
			if (errno != 0 || *ptr != '\0'){
				scale[i] = defScale[i];
			}
		}
	}
}

int main( int argc, char *argv[]){
	int dimensions[2];
	double scale[2];
	paramChecker(argc, argv, dimensions, scale);
	char grid[dimensions[0]][dimensions[1]];
	boardInit(dimensions, grid);
	funcExec(dimensions, scale, grid, argv);
	boardRender(dimensions, grid);
	printf("W %d; H %d; scaleX %f; scaleY %f; funtion %s\n", dimensions[1], dimensions[0], scale[0], scale[1], argv[1]);
}
