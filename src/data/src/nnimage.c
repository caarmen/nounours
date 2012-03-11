/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "nnimage.h"
#include "nncommon.h"
#include "nnmath.h"

NNImage * nnimage_new(NNNounoursApp *app, char *id, char *filename) {
	NNImage *result = malloc(sizeof(NNImage));
	result->adjacent_images = malloc(
			sizeof(NNImage*) * NN_INITIAL_LIST_CAPACITY);
	result->features = malloc(sizeof(NNFeature*) * NN_INITIAL_LIST_CAPACITY);
	result->image_features = malloc(
			sizeof(NNImageFeature*) * NN_INITIAL_LIST_CAPACITY);
	result->id = strdup(id);
	result->filename = filename;
	result->num_features = 0;
	result->num_adjacent_images = 0;
	result->num_image_features = 0;
	result->uiimage = nnuiimage_new(app->ui, filename);

	return result;
}

static NNImageFeature * nnimage_feature_new(NNFeature *feature, int x, int y) {
	NNImageFeature * image_feature = malloc(sizeof(NNImageFeature));
	image_feature->feature = feature;
	image_feature->x = x;
	image_feature->y = y;
	return image_feature;
}

static NNAdjacentImages *nnimage_adjacent_images_new(NNFeature *feature) {
	NNAdjacentImages * adjacent_images = malloc(sizeof(NNAdjacentImages));
	adjacent_images->adjacent_images = malloc(
			sizeof(NNImage*) * NN_INITIAL_LIST_CAPACITY);
	adjacent_images->num_adjacent_images = 0;
	adjacent_images->feature = feature;
	return adjacent_images;
}

void nnimage_add_adjacent_image(NNImage *image, NNFeature *feature,
		NNImage *adjacent_image) {
	NNAdjacentImages *adjacent_images = nnimage_find_adjacent_images(image,
			feature);
	if (adjacent_images == NULL) {
		adjacent_images = nnimage_adjacent_images_new(feature);
		image->adjacent_images = nnresize_if_needed(image->adjacent_images,
				image->num_adjacent_images);
		image->adjacent_images[image->num_adjacent_images++] = adjacent_images;
	}
	adjacent_images->adjacent_images = nnresize_if_needed(
			adjacent_images->adjacent_images,
			adjacent_images->num_adjacent_images);
	adjacent_images->adjacent_images[adjacent_images->num_adjacent_images++] =
			adjacent_image;
}

void nnimage_add_feature(NNImage *image, NNFeature *feature, int x, int y) {
	image->features = nnresize_if_needed(image->features, image->num_features);
	image->features[image->num_features++] = feature;

	image->image_features = nnresize_if_needed(image->image_features,
			image->num_image_features);
	NNImageFeature *image_feature = nnimage_feature_new(feature, x, y);
	image->image_features[image->num_image_features++] = image_feature;
}

NNFeature * nnimage_find_closest_feature(NNImage *image, int x, int y) {
	int min_distance = INT_MAX;
	NNFeature *feature = 0;
	int i;
	for (i = 0; i < image->num_image_features; i++) {
		NNImageFeature *image_feature = image->image_features[i];
		int distance = nnmath_get_distance(image_feature->x, image_feature->y,
				x, y);
		if (distance < min_distance) {
			min_distance = distance;
			feature = image_feature->feature;
		}
	}
	return feature;
}
static int nnimage_get_distance(NNImage *image, NNFeature *feature, int x,
		int y) {
	int i;
	for (i = 0; i < image->num_image_features; i++) {
		if (image->image_features[i]->feature == feature) {
			NNImageFeature *image_feature = image->image_features[i];
			return nnmath_get_distance(image_feature->x, image_feature->y, x, y);
		}
	}
	return INT_MAX;
}

NNAdjacentImages * nnimage_find_adjacent_images(NNImage *image,
		NNFeature *feature) {
	int i;
	for (i = 0; i < image->num_adjacent_images; i++) {
		if (image->adjacent_images[i]->feature->id == feature->id)
			return image->adjacent_images[i];
	}
	return NULL;
}
NNImage * nnimage_find_adjacent_image(NNImage *image, NNFeature *feature, int x,
		int y) {
	NNImage *result = image;
	int min_distance = nnimage_get_distance(image, feature, x, y);
	NNAdjacentImages *adjacent_images = nnimage_find_adjacent_images(image,
			feature);
	if (adjacent_images == NULL)
		return result;
	int i;
	for (i = 0; i < adjacent_images->num_adjacent_images; i++) {
		int distance = nnimage_get_distance(adjacent_images->adjacent_images[i],
				feature, x, y);
		if (distance < min_distance) {
			min_distance = distance;
			result = adjacent_images->adjacent_images[i];
		}
	}
	return result;
}

NNImage * nnimage_get_random_adjacent_image(NNImage *image) {
	int adj_image_number = random() % image->num_adjacent_images;
	NNAdjacentImages *adj_images = image->adjacent_images[adj_image_number];
	int image_number = random() % adj_images->num_adjacent_images;
	return adj_images->adjacent_images[image_number];
}

static void nnimage_adjacent_images_free(NNAdjacentImages *adjacent_images) {
	free(adjacent_images->adjacent_images);
	free(adjacent_images);
}

void nnimage_free(NNImage *image) {
	free(image->id);
	free(image->filename);
	int i;
	for (i = 0; i < image->num_adjacent_images; i++)
		nnimage_adjacent_images_free(image->adjacent_images[i]);
	free(image->adjacent_images);
	for (i = 0; i < image->num_image_features; i++)
		free(image->image_features[i]);
	free(image->image_features);
	free(image->features);
	free(image);
}

