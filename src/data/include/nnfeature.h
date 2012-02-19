/*
 * nfeature.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNFEATURE_H_
#define NNFEATURE_H_

typedef struct NNFeature {
	const char *id;
} NNFeature;

NNFeature * nnfeature_new(const char *id);
void nnfeature_free(NNFeature * nfeature);

#endif /* NNFEATURE_H_ */