/*
 *  Created on: Mar 5, 2012
 *      Author: Carmen Alvarez
 */
#include <stdio.h>
#include <stdlib.h>
#include "nnio.h"

FILE *nnio_open_file(const char *theme_path, const char *file_name,
		const char *mode) {
	char filename[512];
	if (theme_path == 0)
		sprintf(filename, "%s", file_name);
	else
		sprintf(filename, "%s/%s", theme_path, file_name);
	FILE *file = fopen(filename, mode);
	if (file == 0) {
		if (theme_path == 0)
			theme_path = "";
		sprintf(
				filename,
				"%s/nounours/data/themes/%s/%s", __DATAROOT_DIR__, theme_path, file_name);
		file = fopen(filename, "r");
		if (file == 0) {
			fprintf(stderr, "Could not find file %s\n", filename);
			exit(1);
		}
	}
	return file;
}
