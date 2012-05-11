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

#ifndef NNCSVREADER_H_
#define NNCSVREADER_H_
#include "nntheme.h"
#include "nnnounoursapp.h"

void nnread_image_file(NNNounoursApp *app, NNTheme *theme);
void nnread_animation_file(NNTheme *theme);
void nnread_feature_file(NNTheme *theme);
void nnread_image_feature_file(NNTheme *theme);
void nnread_adjacent_image_file(NNTheme *theme);
void nnread_fling_animation_file(NNTheme *theme);

#endif /* NNCSVREADER_H_ */
