/*
 * nimage.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNIMAGE_H_
#define NNIMAGE_H_

#include "nnfeature.h"

typedef struct NNImage {
	const char *id;
	const char *filename;
	int num_adjacent_images;
	struct NNImage **adjacent_images;
	int num_features;
	NNFeature **features;
} NNImage;

typedef struct NNImageFeature {
	NNImage *image;
	NNFeature *feature;
	int x;
	int y;
} NNImageFeature;


NNImage * nnimage_new(const char *id, const char *filename);
void nnimage_add_adjacent_image(NNImage *image, NNImage *adjacent_image);
void nnimage_add_feature(NNImage *image, NNFeature *feature, int x, int y);
void nnimage_free(NNImage *image);
#endif /* NNIMAGE_H_ */
