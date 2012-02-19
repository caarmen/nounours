/*
 * common.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include <errno.h>
#include <stdlib.h>
#include "nncommon.h"
void * nnresize_if_needed(void *ptr, int current_size) {
	if(malloc_size(ptr) <= current_size * sizeof(void*)) {
		ptr = realloc(ptr, (current_size + NN_INITIAL_LIST_CAPACITY)* sizeof(void*));
	}
	return ptr;
}
