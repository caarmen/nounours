/*
 * nncsvreader.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNCSVREADER_H_
#define NNCSVREADER_H_
#include <stdio.h>
#include "nntheme.h"
#include "nnnounours.h"

typedef struct NNCSVLine {
	int num_cells;
	char ** cells;
} NNCSVLine;

NNCSVLine *nncsv_line_new();
void nncsv_line_free(NNCSVLine *line);
FILE *nncsv_pre_read_file(NNTheme *theme, const char *filename, NNCSVLine **header, NNCSVLine **line);
int nncsv_read_line(FILE *file, NNCSVLine *line);
void nncsv_split_line(char *input, NNCSVLine *line, const char *sep);
char *nncsv_get_value(NNCSVLine *header, NNCSVLine *line,
		const char *field_name);
void nncsv_post_read_file(FILE *file, NNCSVLine *header, NNCSVLine *line);

void nnread_image_file(NNTheme *theme);
void nnread_animation_file(NNTheme *theme);
void nnread_feature_file(NNTheme *theme);
void nnread_image_feature_file(NNTheme *theme);
void nnread_adjacent_image_file(NNTheme *theme);
void nnread_fling_animation_file(NNTheme *theme);


#endif /* NNCSVREADER_H_ */
