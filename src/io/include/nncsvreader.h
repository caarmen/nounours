/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNCSVREADER_H_
#define NNCSVREADER_H_
#include <stdio.h>
#include "nntheme.h"
#include "nnnounoursapp.h"

void nnread_image_file(NNNounoursApp *app, NNTheme *theme);
void nnread_animation_file(NNTheme *theme);
void nnread_feature_file(NNTheme *theme);
void nnread_image_feature_file(NNTheme *theme);
void nnread_adjacent_image_file(NNTheme *theme);
void nnread_fling_animation_file(NNTheme *theme);

#endif /* NNCSVREADER_H_ */
