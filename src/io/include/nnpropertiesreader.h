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

#ifndef NNPROPERTIESREADER_H_
#define NNPROPERTIESREADER_H_

#include "nntheme.h"
#include "nnnounoursapp.h"

void nnread_theme_properties_file(NNTheme *theme);
void nnread_nounours_properties_file(NNNounoursConfig *nounours_config,
		const char *path);

#endif /* NNPROPERTIESREADER_H_ */
