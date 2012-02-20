/*
 * nntheme.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdlib.h>
#include <string.h>
#include "nntheme.h"
#include "nnanimation.h"
#include "nnimage.h"
#include "nncommon.h"
NNTheme * nntheme_new(char *id) {
	NNTheme *theme = malloc(sizeof(NNTheme));
	theme->id = strdup(id);
	theme->num_features = 0;
	theme->features = malloc(sizeof(NNFeature*) * NN_INITIAL_LIST_CAPACITY);
	theme->num_images = 0;
	theme->images = malloc(sizeof(NNImage*) *  NN_INITIAL_LIST_CAPACITY);
	theme->num_animations = 0;
	theme->animations = malloc(sizeof(NNAnimation*) * NN_INITIAL_LIST_CAPACITY);
	theme->num_animation_flings = 0;
	theme->animation_flings = malloc(sizeof(NNAnimationFling*) * NN_INITIAL_LIST_CAPACITY);
	theme->shake_animation = 0;
	theme->resume_animation= 0;
	theme->animation_idle = 0;
	theme->animation_idle_end = 0;
	theme->default_image = 0;
	theme->help_image= 0;
	theme->height = 0;
	theme->width = 0;
	return theme;
}

void nntheme_add_feature(NNTheme *theme, NNFeature *feature) {
	theme->features = nnresize_if_needed(theme->features, theme->num_features);
	theme->features[theme->num_features++] = feature;
}

void nntheme_add_animation(NNTheme *theme, NNAnimation *animation) {
	theme->animations = nnresize_if_needed(theme->animations, theme->num_animations);
	theme->animations[theme->num_animations++] = animation;
}

void nntheme_add_animation_fling(NNTheme *theme, NNAnimationFling *animation_fling) {
	theme->animation_flings = nnresize_if_needed(theme->animation_flings, theme->num_animation_flings);
	theme->animation_flings[theme->num_animation_flings++] = animation_fling;
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
NNAnimation *nntheme_find_animation(NNTheme *theme, const char *id) {
	int i;
	for(i=0; i < theme->num_animations; i++)
		if(!strcmp(theme->animations[i]->id, id))
			return theme->animations[i];
	return NULL;
}

NNFeature *nntheme_find_feature(NNTheme *theme, const char *id) {
	int i;
	for(i=0; i < theme->num_features; i++) {
		if(!strcmp(theme->features[i]->id, id))
			return theme->features[i];
	}
	return NULL;
}

void nntheme_free(NNTheme *theme) {
	free(theme->id);
	int i;
	for(i=0; i < theme->num_features; i++)
		nnfeature_free(theme->features[i]);
	for(i=0; i < theme->num_animations; i++)
		nnanimation_free(theme->animations[i]);
	for(i=0; i < theme->num_images; i++)
		nnimage_free(theme->images[i]);
	free(theme->animations);
	free(theme->images);
	free(theme);
}
