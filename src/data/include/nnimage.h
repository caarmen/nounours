/*
 * nimage.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNIMAGE_H_
#define NNIMAGE_H_

#include "nnfeature.h"

typedef struct NNImageFeature {
	NNFeature *feature;
	int x;
	int y;
} NNImageFeature;

typedef struct NNImage {
	char *id;
	char *filename;
	int num_adjacent_images;
	struct NNImage **adjacent_images;
	int num_features;
	NNFeature **features;
	int num_image_features;
	NNImageFeature **image_features;
} NNImage;




NNImage * nnimage_new(char *id, char *filename);
NNImageFeature * nnimage_feature_new(NNFeature *feature, int x, int y);
void nnimage_add_adjacent_image(NNImage *image, NNImage *adjacent_image);
void nnimage_add_feature(NNImage *image, NNFeature *feature, int x, int y);
void nnimage_free(NNImage *image);
#endif /* NNIMAGE_H_ */
