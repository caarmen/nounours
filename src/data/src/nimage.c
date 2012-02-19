/*
 * NImage.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include "nimage.h"
#include <stdlib.h>
NImage * nimage_new(const char *id, const char *filename) {
	NImage *result = malloc(sizeof(NImage));
	result->adjacent_images = malloc(sizeof(NImage*)*10);
	result->id = id;
	result->filename = filename;
	return result;
}

void nimage_add_adjacent_image(NImage *image, NImage *adjacent_image) {

}

void nimage_free(NImage *image) {
	free(image->adjacent_images);
	free(image);
}

