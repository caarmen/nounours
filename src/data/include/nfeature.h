/*
 * nfeature.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NFEATURE_H_
#define NFEATURE_H_

typedef struct NFeature {
	const char *id;
} NFeature;

NFeature * nfeature_new(const char *id);
void nfeature_free(NFeature * nfeature);

#endif /* NFEATURE_H_ */
