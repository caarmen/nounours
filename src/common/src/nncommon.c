/*
 * common.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include "nncommon.h"
void * nnresize_if_needed(void *ptr, int current_size) {
	if((current_size +1) % NN_INITIAL_LIST_CAPACITY == 0) {
		ptr = realloc(ptr, (current_size + NN_INITIAL_LIST_CAPACITY)* sizeof(void*));
	}
	return ptr;
}
