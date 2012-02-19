/*
 * nntheme.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNTHEME_H_
#define NNTHEME_H_
#include "nnimage.h"
#include "nnanimation.h"
typedef struct NNTheme {
	char *id;
	int num_images;
	NNImage **images;
	int num_animations;
	NNAnimation **animations;
} NNTheme;

NNTheme * nntheme_new(char *id);
void nntheme_add_animation(NNTheme *theme, NNAnimation *animation);
void nntheme_add_image(NNTheme *theme, NNImage *image);
NNImage *nntheme_find_image(NNTheme *theme, const char *id);
void nntheme_free(NNTheme *theme);

#endif /* NNTHEME_H_ */
