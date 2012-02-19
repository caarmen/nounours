/*
 * nntheme.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdlib.h>
#include "nntheme.h"
#include "nnanimation.h"
#include "nnimage.h"
#include "nncommon.h"
NNTheme * nntheme_new(char *id) {
	NNTheme *theme = malloc(sizeof(NNTheme));
	theme->id = id;
	theme->num_images = 0;
	theme->images = malloc(sizeof(NNImage*) *  NN_INITIAL_LIST_CAPACITY);
	theme->num_animations = 0;
	theme->animations = malloc(sizeof(NNAnimation*) * NN_INITIAL_LIST_CAPACITY);
	return theme;
}
void nntheme_add_animation(NNTheme *theme, NNAnimation *animation) {
	theme->animations = nnresize_if_needed(theme->animations, theme->num_animations);
	theme->animations[theme->num_animations++] = animation;
}
void nntheme_add_image(NNTheme *theme, NNImage *image) {
	theme->images = nnresize_if_needed(theme->images, theme->num_images);
	theme->images[theme->num_images++] = image;
}
NNImage *nntheme_find_image(NNTheme *theme, const char *id) {
	int i;
	for(i=0; i < theme->num_images; i++) {
		if(!strcmp(theme->images[i]->id, id))
			return theme->images[i];
	}
	return NULL;
}

void nntheme_free(NNTheme *theme) {
	free(theme->id);
	int i;
	for(i=0; i < theme->num_animations; i++)
		nnanimation_free(theme->animations[i]);
	for(i=0; i < theme->num_images; i++)
		nnimage_free(theme->images[i]);
	free(theme->animations);
	free(theme->images);
	free(theme);
}
