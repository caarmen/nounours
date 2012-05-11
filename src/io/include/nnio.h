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

#ifndef NNIO_H_
#define NNIO_H_

#include <stdio.h>

/**
 * Opens a file relative to the working directory, if it exists.  Otherwise, opens
 * a file relative to the data/themes directory of the installed location of
 * the application.
 * @param theme_path if given, we will try to open theme_path/file_name. Otherwise
 * we try to open file_name.
 */
FILE *nnio_open_file(const char *theme_path, const char *file_name,
		const char *mode);

#endif /* NNIO_H_ */
