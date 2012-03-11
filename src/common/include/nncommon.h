/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
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
