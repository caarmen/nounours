/*
 * nimage.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNIMAGE_H_
#define NNIMAGE_H_

#include "nnfeature.h"
#include "nnuiimage.h"
#include "nnnounours.h"

struct NNImage;
struct NNNounours;
struct NNUIImage;

typedef struct NNImageFeature {
	NNFeature *feature;
	int x;
	int y;
} NNImageFeature;

typedef struct NNAdjacentImages {
	NNFeature *feature;
	int num_adjacent_images;
	struct NNImage **adjacent_images;
} NNAdjacentImages;

typedef struct NNImage {
	char *id;
	char *filename;
	int num_adjacent_images;
	NNAdjacentImages **adjacent_images;
	int num_features;
	NNFeature **features;
	int num_image_features;
	NNImageFeature **image_features;
	struct NNUIImage *uiimage;
} NNImage;




NNImage * nnimage_new(struct NNNounours *nounours, char *id, char *filename);
NNImageFeature * nnimage_feature_new(NNFeature *feature, int x, int y);
NNAdjacentImages *nnimage_adjacent_images_new(NNFeature *feature);
void nnimage_add_adjacent_image(NNImage *image, NNFeature *feature, NNImage *adjacent_image);
void nnimage_add_feature(NNImage *image, NNFeature *feature, int x, int y);
NNFeature * nnimage_find_feature(NNImage *image, const char *id);
NNAdjacentImages * nnimage_find_adjacent_images(NNImage *image, NNFeature *feature);
void nnimage_adjacent_images_free(NNAdjacentImages *adjacent_images);
void nnimage_free(NNImage *image);
#endif /* NNIMAGE_H_ */
