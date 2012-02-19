/*
 * NImage.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include "nnimage.h"
#include "nndatacommon.h"

NNImage * nnimage_new(const char *id, const char *filename) {
	NNImage *result = malloc(sizeof(NNImage));
	result->adjacent_images = malloc(sizeof(NNImage*)*NN_INITIAL_LIST_CAPACITY);
	result->features = malloc(sizeof(NNFeature*)*NN_INITIAL_LIST_CAPACITY);
	result->id = id;
	result->filename = filename;
	return result;
}

void nnimage_add_adjacent_image(NNImage *image, NNImage *adjacent_image) {
	nnresize_if_needed(image->adjacent_images, image->num_adjacent_images);
	image->adjacent_images[image->num_adjacent_images] = adjacent_image;
	image->num_adjacent_images++;
}

void nnimage_add_feature(NNImage *image, NNFeature *feature, int x, int y) {
	nnresize_if_needed(image->features, image->num_features);
	image->features[image->num_features] = feature;
	image->num_features++;
	//TODO x,y
}

void nnimage_free(NNImage *image) {
	free(image->adjacent_images);
	free(image);
}

