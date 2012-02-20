/*
 * nntheme.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNTHEME_H_
#define NNTHEME_H_
#include "nnfeature.h"
#include "nnimage.h"
#include "nnanimation.h"
typedef struct NNTheme {
	char *id;
	int num_images;
	NNImage **images;
	int num_animations;
	NNAnimation **animations;
	int num_features;
	NNFeature **features;
	int num_animation_flings;
	NNAnimationFling **animation_flings;
	NNAnimation *shake_animation;
	NNAnimation *resume_animation;
	NNImage *default_image;
	NNImage *help_image;
	int height;
	int width;
	int animation_idle;
	int animation_idle_end;
} NNTheme;

NNTheme * nntheme_new(char *id);
void nntheme_add_feature(NNTheme *theme, NNFeature *feature);
void nntheme_add_animation(NNTheme *theme, NNAnimation *animation);
void nntheme_add_animation_fling(NNTheme *theme, NNAnimationFling *animation_fling);
void nntheme_add_image(NNTheme *theme, NNImage *image);
NNImage *nntheme_find_image(NNTheme *theme, const char *id);
NNAnimation *nntheme_find_animation(NNTheme *theme, const char *id);
NNFeature *nntheme_find_feature(NNTheme *theme, const char *id);
void nntheme_free(NNTheme *theme);

#endif /* NNTHEME_H_ */
