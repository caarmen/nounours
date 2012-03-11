/*
 *  Created on: Feb 20, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNPROPERTIESREADER_H_
#define NNPROPERTIESREADER_H_

#include "nntheme.h"
#include "nnnounoursapp.h"

void nnread_theme_properties_file(NNTheme *theme);
void nnread_nounours_properties_file(NNNounoursConfig *nounours_config, const char *path);

#endif /* NNPROPERTIESREADER_H_ */
