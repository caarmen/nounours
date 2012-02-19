/*
 * NImage.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include "nfeature.h"
#include <stdlib.h>

NFeature * nfeature_new(const char *id) {
	return malloc(sizeof(NFeature*));
}
void nfeature_free(NFeature * nfeature) {
	free(nfeature);
}
