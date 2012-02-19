/*
 * NImage.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include "nnimage.h"
#include <stdlib.h>
NNImage * nnimage_new(const char *id, const char *filename) {
	NNImage *result = malloc(sizeof(NNImage));
	result->adjacent_images = malloc(sizeof(NNImage*)*10);
	result->id = id;
	result->filename = filename;
	return result;
}

void nnimage_add_adjacent_image(NNImage *image, NNImage *adjacent_image) {

}

void nnimage_free(NNImage *image) {
	free(image->adjacent_images);
	free(image);
}

