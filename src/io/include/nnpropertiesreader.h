/*
 * nnpropertiesreader.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNPROPERTIESREADER_H_
#define NNPROPERTIESREADER_H_

#include "nntheme.h"

int nnproperties_read_line(FILE *file, char **key, char **val);

void nnread_theme_file(NNTheme *theme, const char *filename);

#endif /* NNPROPERTIESREADER_H_ */
