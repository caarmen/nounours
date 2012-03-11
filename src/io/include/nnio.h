/*
 *  Created on: Mar 5, 2012
 *      Author: Carmen Alvarez
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
