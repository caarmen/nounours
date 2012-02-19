/*
 * NImage.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include "nnfeature.h"
#include <stdlib.h>

NNFeature * nnfeature_new(const char *id) {
	return malloc(sizeof(NNFeature*));
}
void nnfeature_free(NNFeature * nfeature) {
	free(nfeature);
}
