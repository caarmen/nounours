/*
 *  Created on: Feb 20, 2012
 *      Author: Carmen Alvarez
 */
#include <stdio.h>
#include <string.h>
#include "nnpropertiesreader.h"
#include "nntheme.h"
#include "nnnounours.h"
#include "nncommon.h"
#include "nnio.h"

static int nnproperties_read_line(FILE *file, char **key, char **val) {
	char buf[1024];
	memset(buf, 0, 1024);
	if (!fgets(buf, sizeof(buf), file))
		return 0;
	*val = buf;
	*key = strsep(val, "=");
	*val = strsep(val, "\r\n");
	return 1;
}

void nnread_theme_properties_file(NNTheme *theme) {
	FILE *file = nnio_open_file(theme->path, "theme.properties", "r");

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
		} else if (!strcmp(key, "background.color")) {
			theme->background_color = strdup(val);
		}
	}
	fclose(file);
}

void nnread_nounours_properties_file(NNNounours *nounours, const char *path) {
	FILE *file = nnio_open_file(path, "nounours.properties", "r");
	char *key, *val;
	while (nnproperties_read_line(file, &key, &val)) {
		if (key[0] == '#')
			continue;
		if (!strcmp(key, "shake.factor")) {
			nounours->config.shake_factor = atoi(val);
		} else if (!strcmp(key, "idle.time")) {
			nounours->config.idle_time_for_sleep_ms = atoi(val);
		} else if (!strcmp(key, "idle.ping.interval")) {
			nounours->config.idle_time_for_auto_move_ms = atoi(val);
		}
	}
	fclose(file);
}

