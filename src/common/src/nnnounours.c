/*
 * nnnounours.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include "nnnounours.h"

NNNounours * nnnounours_new() {
	NNNounours *nounours = malloc(sizeof(NNNounours));
	nounours->fling_factor = 0;
	nounours->shake_factor = 0;
	nounours->fling_precision = 0;
	nounours->idle_time = 0;
	nounours->idle_ping_interval = 0;
	return nounours;
}
void nnnounours_free(NNNounours *nounours) {
	free(nounours);
}
