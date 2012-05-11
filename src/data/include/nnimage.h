/*
 * Copyright 2012 Carmen Alvarez
 *
 * This file is part of Nounours.
 *
 * Nounours is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Nounours is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nounours.  If not, see <http://www.gnu.org/licenses/>.
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
