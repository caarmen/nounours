/*
 * nncsvreader.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "nncsvreader.h"
#include "nncommon.h"
#include "nnimage.h"
#include "nnanimation.h"

static const char *CSV_SEP = ",\r\n";

char *nncsv_get_value(NNCSVLine *header, NNCSVLine *line,
		const char *field_name) {
	int i;
	for (i = 0; i < header->num_cells; i++) {
		if (!strcmp(field_name, header->cells[i]))
			return line->cells[i];
	}
	return "";
}

NNCSVLine *nncsv_line_new() {
	NNCSVLine *line = malloc(sizeof(NNCSVLine));
	line->num_cells = 0;
	line->cells = malloc(sizeof(char*) * NN_INITIAL_LIST_CAPACITY);
	return line;
}
void nncsv_free(NNCSVLine *line) {
	free(line->cells);
	free(line);
}

void nncsv_split_line(char *input, NNCSVLine *line, const char *sep) {
	char *ptr;
	char *cell;
	line->num_cells = 0;
	for (cell = strtok_r(input, sep, &ptr); cell != NULL;
			cell = strtok_r(NULL, sep, &ptr)) {
		line->cells = nnresize_if_needed(line->cells, line->num_cells);
		line->cells[line->num_cells++] = strdup(cell);
		// TODO need to free unused cells!
	}
}

int nncsv_read_line(FILE *file, NNCSVLine *line) {
	char buf[1024];
	if (!fgets(buf, sizeof(buf), file))
		return 0;
	nncsv_split_line(buf, line, CSV_SEP);
	return 1;
}

FILE *nncsv_pre_read_file(const char *filename, NNCSVLine **header,
		NNCSVLine **line) {
	FILE *file = fopen(filename, "r");
	*header = nncsv_line_new();
	nncsv_read_line(file, *header);
	*line = nncsv_line_new();
	return file;
}

void nncsv_post_read_file(FILE *file, NNCSVLine *header, NNCSVLine *line) {
	nncsv_free(header);
	nncsv_free(line);
	fclose(file);
}

void nnread_image_file(NNTheme *theme, const char *filename) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(filename, &header, &line);
	while (nncsv_read_line(file, line)) {
		char *id = nncsv_get_value(header, line, "Id");
		char *image_filename = nncsv_get_value(header, line, "Filename");
		NNImage *image = nnimage_new(id, image_filename);
		nntheme_add_image(theme, image);
	}
	nncsv_post_read_file(file, header, line);
}

void nnread_animation_file(NNTheme *theme, const char *filename) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(filename, &header, &line);
	NNCSVLine *image_ids = nncsv_line_new();
	while (nncsv_read_line(file, line)) {
		char *id = nncsv_get_value(header, line, "Id");
		char *label = nncsv_get_value(header, line, "Label");
		char *interval = nncsv_get_value(header, line, "Interval");
		char *repeat = nncsv_get_value(header, line, "Repeat");
		NNAnimation *animation = nnanimation_new(id, label, atoi(interval),
				atoi(repeat));
		char *sequence = nncsv_get_value(header, line, "Sequence");
		nncsv_split_line(sequence, image_ids, ";");
		int i;
		float duration = 1.0f;
		for (i = 0; i < image_ids->num_cells; i++) {
			const char *cell = image_ids->cells[i];
			if (strnstr(cell, "d=", 2)) {
				duration = strtof(&cell[2], NULL);
			} else {
				NNImage *image = nntheme_find_image(theme, image_ids->cells[i]);
				nnanimation_add_image(animation, image, duration);
			}
		}
		nntheme_add_animation(theme, animation);
		free(interval);
		free(repeat);
		free(sequence);
	}
	nncsv_post_read_file(file, header, line);
}

void nnread_feature_file(NNTheme *theme, const char *filename) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(filename, &header, &line);
	while (nncsv_read_line(file, line)) {
		char *id = nncsv_get_value(header, line, "Id");
		NNFeature *feature = nnfeature_new(id);
		nntheme_add_feature(theme, feature);
	}
	nncsv_post_read_file(file, header, line);
}

void nnread_image_feature_file(NNTheme *theme, const char *filename) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(filename, &header, &line);
	while (nncsv_read_line(file, line)) {
		char *image_id = nncsv_get_value(header, line, "ImageId");
		char *feature_id = nncsv_get_value(header, line, "FeatureId");
		char *x = nncsv_get_value(header, line, "X");
		char *y = nncsv_get_value(header, line, "Y");
		NNImage *image = nntheme_find_image(theme, image_id);
		NNFeature *feature = nntheme_find_feature(theme, feature_id);
		nnimage_add_feature(image, feature, atoi(x), atoi(y));
		free(x);
		free(y);
	}
	nncsv_post_read_file(file, header, line);
}

void nnread_adjacent_image_file(NNTheme *theme, const char *filename) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(filename, &header, &line);
	while (nncsv_read_line(file, line)) {
		char *image_id = nncsv_get_value(header, line, "ImageId");
		char *feature_id = nncsv_get_value(header, line, "FeatureId");
		char *adjacent_image_id = nncsv_get_value(header, line,
				"AdjacentImageId");
		NNImage *image = nntheme_find_image(theme, image_id);
		NNFeature *feature = nntheme_find_feature(theme, feature_id);
		NNImage *adjacent_image = nntheme_find_image(theme, adjacent_image_id);
		nnimage_add_adjacent_image(image, adjacent_image);
		//TODO adjacent image needs feature
	}
	nncsv_post_read_file(file, header, line);
}

void nnread_fling_animation_file(NNTheme *theme, const char *filename) {
	NNCSVLine *header, *line;
	FILE *file = nncsv_pre_read_file(filename, &header, &line);
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
