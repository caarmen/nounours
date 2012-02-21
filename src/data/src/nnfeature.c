/*
 * NImage.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include "nnfeature.h"
#include <stdlib.h>
#include <string.h>

NNFeature * nnfeature_new(char *id) {
	NNFeature *feature = malloc(sizeof(NNFeature*));
	feature->id = strdup(id);
	return feature;
}
void nnfeature_free(NNFeature * nfeature) {
	free(nfeature->id);
	free(nfeature);
}
