/*
 *  Created on: Feb 20, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNPROPERTIESREADER_H_
#define NNPROPERTIESREADER_H_

#include <stdio.h>
#include "nntheme.h"
#include "nnnounours.h"

void nnread_theme_properties_file(NNTheme *theme);
void nnread_nounours_properties_file(NNNounours *nounours, const char *path);

#endif /* NNPROPERTIESREADER_H_ */
