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

#ifndef NNCOMMON_H_
#define NNCOMMON_H_

#define NN_INITIAL_LIST_CAPACITY 10

#ifndef __DATAROOT_DIR__
#define __DATAROOT_DIR__ "/usr/local/share"
#endif

typedef unsigned char nnbool; //Yes, I know this is lame.
#define NNTRUE 1
#define NNFALSE 0

/**
 * Allocate more memory to the pointer ptr, if it is currently at full capacity.
 * @param ptr points to an array of pointers.
 * @param current_size the current number of elements in the array.
 */
void * nnresize_if_needed(void *ptr, int current_size);
#endif /* NNCOMMON_H_ */
