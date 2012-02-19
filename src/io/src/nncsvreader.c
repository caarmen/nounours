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


const char *nncsv_get_value(NNCSVLine *header, NNCSVLine *line, const char *field_name) {
	int i;
	for(i=0; i < header->num_cells; i++){
		if(!strcmp(field_name, header->cells[i]))
			return line->cells[i];
	}
	return "";
}

NNCSVLine *nncsv_line_new() {
	NNCSVLine *line = malloc(sizeof(NNCSVLine));
	line->num_cells = 0;
	line->cells = malloc(sizeof(char*)*NN_INITIAL_LIST_CAPACITY);
	return line;
}
void nncsv_free(NNCSVLine *line) {
	free(line->cells);
	free(line);
}
int nncsv_read_line(FILE *file, NNCSVLine *line){
	char buf[1024];
	if(!fgets(buf, sizeof(buf), file))
		return 0;
	char *ptr;
	char *cell;
	line->num_cells = 0;
	for (cell = strtok_r(buf, CSV_SEP, &ptr); cell != NULL;
			cell = strtok_r(NULL, CSV_SEP, &ptr)) {
		line->cells = nnresize_if_needed(line->cells, line->num_cells);
		line->cells[line->num_cells++] = strdup(cell);
	}
	return 1;
}


void nnread_image_file(NNTheme *theme, const char *filename) {
	FILE *file = fopen(filename, "r");
	char buf[200];
	NNCSVLine *header = nncsv_line_new();
	nncsv_read_line(file, header);
	NNCSVLine *line = nncsv_line_new();
	while(nncsv_read_line(file, line)) {
		const char *id = nncsv_get_value(header, line, "Id");
		const char *image_filename = nncsv_get_value(header, line, "Filename");
		NNImage *image = nnimage_new(id, image_filename);
		nntheme_add_image(theme, image);
	}
	nncsv_free(header);
	nncsv_free(line);
	fclose(file);
}

void nnread_animation_file(NNTheme *theme, const char *filename) {
	FILE *file = fopen(filename, "r");
	char buf[200];
	NNCSVLine *header = nncsv_line_new();
	nncsv_read_line(file, header);
	NNCSVLine *line = nncsv_line_new();
	while(nncsv_read_line(file, line)) {
		const char *id = nncsv_get_value(header, line, "Id");
		const char *label = nncsv_get_value(header, line, "Label");
		const char *interval = nncsv_get_value(header, line, "Interval");
		const char *repeat = nncsv_get_value(header, line, "Repeat");
		NNAnimation *animation = nnanimation_new(id, label, atoi(interval), atoi(repeat));
		nntheme_add_animation(theme, animation);
		//TODO image seq
	}
	nncsv_free(header);
	nncsv_free(line);
	fclose(file);
}
