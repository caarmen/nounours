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
	result->image_features = malloc(sizeof(NNImageFeature*)*NN_INITIAL_LIST_CAPACITY);
	result->id = id;
	result->filename = filename;
	return result;
}

NNImageFeature * nnimage_feature_new(NNFeature *feature, int x, int y) {
	NNImageFeature * image_feature = malloc(sizeof(NNImageFeature));
	image_feature->feature = feature;
	image_feature->x = x;
	image_feature->y = y;
	return image_feature;
}

void nnimage_add_adjacent_image(NNImage *image, NNImage *adjacent_image) {
	nnresize_if_needed(image->adjacent_images, image->num_adjacent_images);
	image->adjacent_images[image->num_adjacent_images++] = adjacent_image;
}

void nnimage_add_feature(NNImage *image, NNFeature *feature, int x, int y) {
	nnresize_if_needed(image->features, image->num_features);
	image->features[image->num_features++] = feature;

	nnresize_if_needed(image->image_features, image->num_image_features);
	NNImageFeature *image_feature = nnimage_feature_new(feature, x, y);
	image->image_features[image->num_image_features++] = image_feature;
}

void nnimage_free(NNImage *image) {
	free(image->adjacent_images);
	int i;
	for(i=0; i < image->num_image_features; i++)
		free(image->image_features[i]);
	free(image->image_features);
	free(image->features);
	free(image);
}
