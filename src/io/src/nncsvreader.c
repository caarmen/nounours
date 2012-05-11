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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "nncsvreader.h"
#include "nncommon.h"
#include "nnimage.h"
#include "nnanimation.h"
#include "nnio.h"

static const char *CSV_SEP = ",\r\n";

typedef struct NNCSVLine {
	int num_cells;
	char ** cells;
} NNCSVLine;

static char *nncsv_get_value(NNCSVLine *header, NNCSVLine *line,
		const char *field_name) {
	int i;
	for (i = 0; i < line->num_cells; i++) {
		if (!strcmp(field_name, header->cells[i]))
			return line->cells[i];
	}
	return "";
}

static NNCSVLine *nncsv_line_new() {
	NNCSVLine *line = malloc(sizeof(NNCSVLine));
	line->num_cells = 0;
	line->cells = malloc(sizeof(char*) * NN_INITIAL_LIST_CAPACITY);
	return line;
}

static void nncsv_line_free(NNCSVLine *line) {
	int i;
	for (i = 0; i < line->num_cells; i++)
		free(line->cells[i]);
	free(line->cells);
	free(line);
}

static void nncsv_split_line(char *input, NNCSVLine *line, const char *sep) {
	char *ptr;
	char *cell;
	line->num_cells = 0;
	for (cell = strtok_r(input, sep, &ptr); cell != NULL;
			cell = strtok_r(NULL, sep, &ptr)) {
		line->cells = nnresize_if_needed(line->cells, line->num_cells);
		line->cells[line->num_cells++] = strdup(cell);
	}
}

static int nncsv_read_line(FILE *file, NNCSVLine *line) {
	char buf[1024];
	if (!fgets(buf, sizeof(buf), file))
		return 0;
	nncsv_split_line(buf, line, CSV_SEP);
	return 1;
}

static FILE *nncsv_pre_read_file(NNTheme *theme, const char *filename,
		NNCSVLine **header, NNCSVLine **line) {
	FILE *file = nnio_open_file(theme->path, filename, "r");
	*header = nncsv_line_new();
	nncsv_read_line(file, *header);
	*line = nncsv_line_new();
	return file;
}

static void nncsv_post_read_file(FILE *file, NNCSVLine *header, NNCSVLine *line) {
	nncsv_line_free(header);
	nncsv_line_free(line);
	fclose(file);
}

void nnread_image_file(NNNounoursApp *app, NNTheme *theme) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(theme, "image.csv", &header, &line);
	while (nncsv_read_line(file, line)) {
		char *id = nncsv_get_value(header, line, "Id");
		char *image_filename = nncsv_get_value(header, line, "Filename");
		char location[128];
		sprintf(location, "%s/images/%s", theme->path, image_filename);

		NNImage *image = nnimage_new(app, id, strdup(location));
		char *release_image_id = nncsv_get_value(header, line, "OnRelease");
		NNImage *release = nntheme_find_image(theme, release_image_id);
		image->release = release;
		nntheme_add_image(theme, image);
	}
	nncsv_post_read_file(file, header, line);
}

void nnread_animation_file(NNTheme *theme) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(theme, "animation.csv", &header, &line);
	NNCSVLine *image_ids = nncsv_line_new();
	while (nncsv_read_line(file, line)) {
		char *id = nncsv_get_value(header, line, "Id");
		char *label = nncsv_get_value(header, line, "Label");
		char *interval = nncsv_get_value(header, line, "Interval");
		char *repeat = nncsv_get_value(header, line, "Repeat");
		NNAnimation *animation = nnanimation_new(id, label, atoi(interval),
				atoi(repeat));
		animation->is_preset = NNTRUE;
		char *sequence = nncsv_get_value(header, line, "Sequence");
		nncsv_split_line(sequence, image_ids, ";");
		int i;
		float duration = 1.0f;
		for (i = 0; i < image_ids->num_cells; i++) {
			const char *cell = image_ids->cells[i];
			if (strstr(cell, "d=")) {
				duration = strtof(&cell[2], NULL);
			} else {
				NNImage *image = nntheme_find_image(theme, image_ids->cells[i]);
				nnanimation_add_image(animation, image, duration);
			}
		}
		nntheme_add_animation(theme, animation);
	}
	free(image_ids);
	nncsv_post_read_file(file, header, line);
}

void nnread_feature_file(NNTheme *theme) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(theme, "feature.csv", &header, &line);
	while (nncsv_read_line(file, line)) {
		char *id = nncsv_get_value(header, line, "Id");
		NNFeature *feature = nnfeature_new(id);
		nntheme_add_feature(theme, feature);
	}
	nncsv_post_read_file(file, header, line);
}

void nnread_image_feature_file(NNTheme *theme) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(theme, "imagefeatureassoc.csv", &header,
			&line);
	while (nncsv_read_line(file, line)) {
		char *image_id = nncsv_get_value(header, line, "ImageId");
		char *feature_id = nncsv_get_value(header, line, "FeatureId");
		char *x = nncsv_get_value(header, line, "X");
		char *y = nncsv_get_value(header, line, "Y");
		NNImage *image = nntheme_find_image(theme, image_id);
		NNFeature *feature = nntheme_find_feature(theme, feature_id);
		nnimage_add_feature(image, feature, atoi(x), atoi(y));
	}
	nncsv_post_read_file(file, header, line);
}

void nnread_adjacent_image_file(NNTheme *theme) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(theme, "adjacentimage.csv", &header,
			&line);
	while (nncsv_read_line(file, line)) {
		char *image_id = nncsv_get_value(header, line, "ImageId");
		char *feature_id = nncsv_get_value(header, line, "FeatureId");
		char *adjacent_image_id = nncsv_get_value(header, line,
				"AdjacentImageId");
		NNImage *image = nntheme_find_image(theme, image_id);
		NNFeature *feature = nntheme_find_feature(theme, feature_id);
		NNImage *adjacent_image = nntheme_find_image(theme, adjacent_image_id);
		nnimage_add_adjacent_image(image, feature, adjacent_image);
		nnimage_add_adjacent_image(adjacent_image, feature, image);
	}
	nncsv_post_read_file(file, header, line);
}

void nnread_fling_animation_file(NNTheme *theme) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(theme, "flinganimation.csv", &header,
			&line);
	while (nncsv_read_line(file, line)) {
		char *id = nncsv_get_value(header, line, "Id");
		char *x = nncsv_get_value(header, line, "X");
		char *y = nncsv_get_value(header, line, "Y");
		char *width = nncsv_get_value(header, line, "Width");
		char *height = nncsv_get_value(header, line, "Height");
		char *min_vel_x = nncsv_get_value(header, line, "MinVelX");
		char *min_vel_y = nncsv_get_value(header, line, "MinVelY");
		char *animation_id = nncsv_get_value(header, line, "AnimationId");
		NNAnimation *animation = nntheme_find_animation(theme, animation_id);
		NNAnimationFling *fling = nnanimation_fling_new(atoi(x), atoi(y),
				atoi(width), atoi(height), strtof(min_vel_x, NULL),
				strtof(min_vel_y, NULL), animation);
		nntheme_add_animation_fling(theme, fling);
	}
	nncsv_post_read_file(file, header, line);
}
