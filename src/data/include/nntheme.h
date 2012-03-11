/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNTHEME_H_
#define NNTHEME_H_
#include "nnfeature.h"
#include "nnimage.h"
#include "nnanimation.h"
#include "nnnounours.h"
typedef struct NNTheme {
	NNNounours *nounours;
	char *path;
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
	NNAnimation *animation_idle;
	NNAnimation *animation_idle_end;
	NNImage *default_image;
	NNImage *help_image;
	int height;
	int width;
	char *background_color;
} NNTheme;

NNTheme * nntheme_new(NNNounours *nounours, char *path);
void nntheme_add_feature(NNTheme *theme, NNFeature *feature);
void nntheme_add_animation(NNTheme *theme, NNAnimation *animation);
void nntheme_add_animation_fling(NNTheme *theme,
		NNAnimationFling *animation_fling);
void nntheme_add_image(NNTheme *theme, NNImage *image);
NNImage *nntheme_find_image(NNTheme *theme, const char *id);
NNAnimation *nntheme_find_animation(NNTheme *theme, const char *id);
NNFeature *nntheme_find_feature(NNTheme *theme, const char *id);
void nntheme_free(NNTheme *theme);
char * nntheme_get_random_theme_id();
void nntheme_get_theme_ids(char ***theme_ids, int *num_themes);

#endif /* NNTHEME_H_ */
