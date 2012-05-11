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
#include <stdio.h>
#include <string.h>
#include "nnpropertiesreader.h"
#include "nntheme.h"
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

void nnread_nounours_properties_file(NNNounoursConfig *nounours_config,
		const char *path) {
	FILE *file = nnio_open_file(path, "nounours.properties", "r");
	char *key, *val;
	while (nnproperties_read_line(file, &key, &val)) {
		if (key[0] == '#')
			continue;
		if (!strcmp(key, "shake.factor")) {
			nounours_config->shake_factor = atoi(val);
		} else if (!strcmp(key, "idle.time")) {
			nounours_config->idle_time_for_sleep_ms = atoi(val);
		} else if (!strcmp(key, "idle.ping.interval")) {
			nounours_config->idle_time_for_auto_move_ms = atoi(val);
		}
	}
	fclose(file);
}

