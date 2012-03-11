/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNTHEME_H_
#define NNTHEME_H_
#include "nnfeature.h"
#include "nnimage.h"
#include "nnanimation.h"
#include "nnnounoursapp.h"

struct NNNounours;
struct NNImage;
struct NNAnimation;
struct NNAnimationFling;
struct NNNounoursApp;

typedef struct NNTheme {
	char *path;
	int num_images;
	struct NNImage **images;
	int num_animations;
	struct NNAnimation **animations;
	int num_features;
	NNFeature **features;
	int num_animation_flings;
	struct NNAnimationFling **animation_flings;
	struct NNAnimation *shake_animation;
	struct NNAnimation *resume_animation;
	struct NNAnimation *animation_idle;
	struct NNAnimation *animation_idle_end;
	struct NNImage *default_image;
	struct NNImage *help_image;
	int height;
	int width;
	char *background_color;
} NNTheme;

NNTheme * nntheme_new(struct NNNounoursApp *app, char *path);
void nntheme_add_feature(NNTheme *theme, NNFeature *feature);
void nntheme_add_animation(NNTheme *theme, struct NNAnimation *animation);
void nntheme_add_animation_fling(NNTheme *theme,
		struct NNAnimationFling *animation_fling);
void nntheme_add_image(NNTheme *theme, struct NNImage *image);
struct NNImage *nntheme_find_image(NNTheme *theme, const char *id);
struct NNAnimation *nntheme_find_animation(NNTheme *theme, const char *id);
NNFeature *nntheme_find_feature(NNTheme *theme, const char *id);
void nntheme_free(NNTheme *theme);
char * nntheme_get_random_theme_id();
void nntheme_get_theme_ids(char ***theme_ids, int *num_themes);

#endif /* NNTHEME_H_ */
