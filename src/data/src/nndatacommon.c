/*
 * common.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include "nndatacommon.h"

void nnresize_if_needed(void *ptr, int current_size) {
	if(sizeof(ptr) >= current_size * sizeof(void*)) {
		realloc(ptr, (current_size + NN_INITIAL_LIST_CAPACITY)* sizeof(void*));
	}
}
