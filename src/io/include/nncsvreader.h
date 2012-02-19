/*
 * nncsvreader.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNCSVREADER_H_
#define NNCSVREADER_H_
#include "nntheme.h"

typedef struct NNCSVLine {
	int num_cells;
	const char ** cells;
} NNCSVLine;

NNCSVLine *nncsv_line_new();
int nncsv_read_line(FILE *file, NNCSVLine *line);
const char *nncsv_get_value(NNCSVLine *header, NNCSVLine *line,
		const char *field_name);
void nncsv_free(NNCSVLine *line);


void nnread_image_file(NNTheme *theme, const char *filename);
void nnread_animation_file(NNTheme *theme, const char *filename);

#endif /* NNCSVREADER_H_ */
