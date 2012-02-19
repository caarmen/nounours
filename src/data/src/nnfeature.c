/*
 * NImage.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include "nnfeature.h"
#include <stdlib.h>

NNFeature * nnfeature_new(const char *id) {
	NNFeature *feature = malloc(sizeof(NNFeature*));
	feature->id = id;
	return feature;
}
void nnfeature_free(NNFeature * nfeature) {
	free(nfeature);
}
