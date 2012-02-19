/*
 * main.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdio.h>
#include "nnimage.h"
int main(int argc, char **argv) {
	NNImage *image = nnimage_new("someid", "somefile");
	printf("Hello %s\n", image->filename);
	return 0;
}
