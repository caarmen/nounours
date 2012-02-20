/*
 * NImage.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include "nnimage.h"
#include "nncommon.h"

NNImage * nnimage_new(char *id, char *filename) {
	NNImage *result = malloc(sizeof(NNImage));
	result->adjacent_images = malloc(sizeof(NNImage*)*NN_INITIAL_LIST_CAPACITY);
	result->features = malloc(sizeof(NNFeature*)*NN_INITIAL_LIST_CAPACITY);
	result->image_features = malloc(sizeof(NNImageFeature*)*NN_INITIAL_LIST_CAPACITY);
	result->id = id;
	result->filename = filename;
	result->num_features = 0;
	result->num_adjacent_images = 0;
	result->num_image_features = 0;

	return result;
}

NNImageFeature * nnimage_feature_new(NNFeature *feature, int x, int y) {
	NNImageFeature * image_feature = malloc(sizeof(NNImageFeature));
	image_feature->feature = feature;
	image_feature->x = x;
	image_feature->y = y;
	return image_feature;
}

NNAdjacentImages *nnimage_adjacent_images_new(NNFeature *feature) {
	NNAdjacentImages * adjacent_images = malloc(sizeof(NNAdjacentImages));
	adjacent_images->adjacent_images = malloc(sizeof(NNImage*) * NN_INITIAL_LIST_CAPACITY);
	adjacent_images->feature = feature;
	return adjacent_images;
}

void nnimage_add_adjacent_image(NNImage *image, NNFeature *feature, NNImage *adjacent_image) {
	NNAdjacentImages *adjacent_images = nnimage_find_adjacent_images(image, feature);
	if(adjacent_images == NULL) {
		adjacent_images = nnimage_adjacent_images_new(feature);
		image->adjacent_images = nnresize_if_needed(image->adjacent_images, image->num_adjacent_images);
		image->adjacent_images[image->num_adjacent_images++] = adjacent_images;
	}
	adjacent_images->adjacent_images = nnresize_if_needed(adjacent_images->adjacent_images, adjacent_images->num_adjacent_images);
	adjacent_images->adjacent_images[adjacent_images->num_adjacent_images++] = adjacent_image;
}

void nnimage_add_feature(NNImage *image, NNFeature *feature, int x, int y) {
	image->features = nnresize_if_needed(image->features, image->num_features);
	image->features[image->num_features++] = feature;

	image->image_features = nnresize_if_needed(image->image_features, image->num_image_features);
	NNImageFeature *image_feature = nnimage_feature_new(feature, x, y);
	image->image_features[image->num_image_features++] = image_feature;
}

NNFeature * nnimage_find_feature(NNImage *image, const char *id) {
	int i;
	for(i=0; i < image->num_features; i++) {
		if(!strcmp(image->features[i]->id, id)) {
			return image->features[i];
		}
	}
	return NULL;
}
NNAdjacentImages * nnimage_find_adjacent_images(NNImage *image, NNFeature *feature) {
	int i;
	for(i=0; i < image->num_adjacent_images; i++) {
		if(image->adjacent_images[i]->feature == feature)
			return image->adjacent_images[i];
	}
	return NULL;
}
void nnimage_adjacent_images_free(NNAdjacentImages *adjacent_images) {
	free(adjacent_images->adjacent_images);
	free(adjacent_images);
}

void nnimage_free(NNImage *image) {
	free(image->id);
	free(image->filename);
	free(image->adjacent_images);
	int i;
	for(i=0; i < image->num_image_features; i++)
		free(image->image_features[i]);
	free(image->image_features);
	free(image->features);
	free(image);
}

