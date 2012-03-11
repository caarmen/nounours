/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
 */
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <syslog.h>
#include "nntheme.h"
#include "nnanimation.h"
#include "nnimage.h"
#include "nncommon.h"
#include "nncsvreader.h"

NNTheme * nntheme_new(NNNounours *nounours, char *path) {
	NNTheme *theme = malloc(sizeof(NNTheme));
	theme->nounours = nounours;
	theme->path = strdup(path);
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
	theme->background_color = 0;
	nnread_feature_file(theme);
	nnread_image_file(theme);
	nnread_animation_file(theme);
	nnread_image_feature_file(theme);
	nnread_adjacent_image_file(theme);
	nnread_fling_animation_file(theme);
	nnread_theme_properties_file(theme);

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
	free(theme->path);
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
	if(theme->background_color)
		free(theme->background_color);
}
char * nntheme_get_random_theme_id() {
	int num_themes;
	char **theme_ids;
	nntheme_get_theme_ids(&theme_ids, &num_themes);
	if(num_themes == 0) {
		return NULL;
	}
	int random_theme_idx = random() % num_themes;
	syslog(LOG_DEBUG, "Randomly choosing theme %s", theme_ids[random_theme_idx]);
	return theme_ids[random_theme_idx];
}
void nntheme_get_theme_ids(char ***theme_ids_return, int *num_themes_return) {
	*theme_ids_return = NULL;
	*num_themes_return = 0;
	char **theme_ids = (char**) malloc(NN_INITIAL_LIST_CAPACITY * sizeof(char*));
	int num_themes = 0;
	char themes_path[1024];
	sprintf(themes_path, "%s/nounours/data/themes/", __DATAROOT_DIR__);
	DIR *themes_dir = opendir(themes_path);
	if(themes_dir == NULL) {
		fprintf(stderr, "Could not find %s!\n", themes_path);
		return;
	}
	struct dirent *theme_dir;
	while((theme_dir = readdir(themes_dir))!=NULL) {
		if(!strcmp(".", theme_dir->d_name) || !strcmp("..", theme_dir->d_name))
			continue;
		theme_ids = nnresize_if_needed(theme_ids, num_themes);
		char *theme_dir_name = strdup(theme_dir->d_name); // TODO should free this
		theme_ids[num_themes++] = theme_dir_name;
	}
	closedir(themes_dir);
	*theme_ids_return = theme_ids;
	*num_themes_return = num_themes;
}
