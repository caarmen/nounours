/*
 * nnpropertiesreader.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNPROPERTIESREADER_H_
#define NNPROPERTIESREADER_H_

#include "nntheme.h"
#include "nnnounours.h"

int nnproperties_read_line(FILE *file, char **key, char **val);

void nnread_theme_properties_file(NNTheme *theme, const char *filename);
void nnread_nounours_properties_file(NNNounours *nounours, const char *filename);

#endif /* NNPROPERTIESREADER_H_ */
