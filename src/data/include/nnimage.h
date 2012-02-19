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
	struct NNImage **adjacent_images;
} NNImage;

typedef struct NNImageFeature {
	NNImage *image;
	NNFeature *feature;
	int x;
	int y;
} NNImageFeature;

NNFeature * nnfeature_new(const char *id);
void nnfeature_free(NNFeature * nfeature);
NNImage * nnimage_new(const char *id, const char *filename);
void nnimage_add_adjacent_image(NNImage *image, NNImage *adjacent_image);
void nnimage_free(NNImage *image);
#endif /* NNIMAGE_H_ */
