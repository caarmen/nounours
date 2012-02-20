/*
 * nnpropertiesreader.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */
#include <stdio.h>
#include <string.h>
#include "nnpropertiesreader.h"
#include "nntheme.h"
#include "nnnounours.h"

int nnproperties_read_line(FILE *file, char **key, char **val) {
	char buf[1024];
	memset(buf, 0, 1024);
	if (!fgets(buf, sizeof(buf), file))
		return 0;
	*val = buf;
	*key = strsep(val, "=");
	*val = strsep(val, "\r\n");
	return 1;
}
void nnread_theme_properties_file(NNTheme *theme, const char *filename) {
	FILE *file = fopen(filename, "r");
	char *key, *val;
	while (nnproperties_read_line(file, &key, &val)) {
		if (key[0] == '#')
			continue;
		if (!strcmp(key, "animation.shake")) {
			NNAnimation *animation = nntheme_find_animation(theme, val);
			theme->shake_animation = animation;
		} else if (!strcmp(key, "animation.resume")) {
			NNAnimation *animation = nntheme_find_animation(theme, val);
			theme->resume_animation = animation;
		} else if (!strcmp(key, "animation.idle")) {
			NNAnimation *animation = nntheme_find_animation(theme, val);
			theme->animation_idle = animation;
		} else if (!strcmp(key, "animation.idle.end")) {
			NNAnimation *animation = nntheme_find_animation(theme, val);
			theme->animation_idle_end = animation;
		} else if (!strcmp(key, "help.image")) {
			NNImage *image = nntheme_find_image(theme, val);
			theme->help_image = image;
		} else if (!strcmp(key, "default.image")) {
			NNImage *image = nntheme_find_image(theme, val);
			theme->default_image = image;
		} else if (!strcmp(key, "resolution.height")) {
			theme->height = atoi(val);
		} else if (!strcmp(key, "resolution.width")) {
			theme->width = atoi(val);
		}
	}
	fclose(file);
}

void nnread_nounours_properties_file(NNNounours *nounours, const char *filename) {
	FILE *file = fopen(filename, "r");
	char *key, *val;
	while (nnproperties_read_line(file, &key, &val)) {
		if (key[0] == '#')
			continue;
		if (!strcmp(key, "fling.factor")) {
			nounours->fling_factor = atoi(val);
		} else if (!strcmp(key, "fling.precision")) {
			nounours->fling_factor = atoi(val);
		} else if (!strcmp(key, "shake.factor")) {
			nounours->shake_factor = atoi(val);
		} else if (!strcmp(key, "idle.time")) {
			nounours->idle_time = atoi(val);
		} else if (!strcmp(key, "idle.ping.interval")) {
			nounours->idle_ping_interval = atoi(val);
		}
	}
	fclose(file);
}

