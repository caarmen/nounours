/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNIMAGE_H_
#define NNIMAGE_H_

#include "nnfeature.h"
#include "nnuiimage.h"
#include "nnnounoursapp.h"

struct NNImage;
struct NNNounours;
struct NNUIImage;
struct NNNounoursApp;

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
	struct NNImage *release;
	struct NNUIImage *uiimage;
} NNImage;

NNImage * nnimage_new(struct NNNounoursApp *app, char *id, char *filename);

void nnimage_add_adjacent_image(NNImage *image, NNFeature *feature,
		NNImage *adjacent_image);

void nnimage_add_feature(NNImage *image, NNFeature *feature, int x, int y);

NNFeature * nnimage_find_closest_feature(NNImage *image, int x, int y);

NNAdjacentImages * nnimage_find_adjacent_images(NNImage *image,
		NNFeature *feature);

NNImage * nnimage_find_adjacent_image(NNImage *image, NNFeature *feature, int x,
		int y);

NNImage * nnimage_get_random_adjacent_image(NNImage *image);

void nnimage_free(NNImage *image);
#endif /* NNIMAGE_H_ */
