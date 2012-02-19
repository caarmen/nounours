/*
 * nnanimation.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include "nnanimation.h"
#include "stdlib.h"

NNAnimation * nnanimation_new(const char *id, const char *label, int interval, int repeat) {
	NNAnimation *animation = malloc(sizeof(NNAnimation));
	animation->id = id;
	animation->label = label;
	animation->interval = interval;
	animation->repeat = repeat;
}

void nnanimation_free(NNAnimation *animation) {
	free(animation);
}

