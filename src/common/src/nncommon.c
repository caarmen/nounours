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

#include <stdlib.h>
#include "nncommon.h"
void * nnresize_if_needed(void *ptr, int current_size) {
	if ((current_size + 1) % NN_INITIAL_LIST_CAPACITY == 0) {
		ptr = realloc(ptr,
				(current_size + NN_INITIAL_LIST_CAPACITY) * sizeof(void*));
	}
	return ptr;
}
