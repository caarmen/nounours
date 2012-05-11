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
