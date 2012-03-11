/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNFEATURE_H_
#define NNFEATURE_H_

typedef struct NNFeature {
	char *id;
} NNFeature;

NNFeature * nnfeature_new(char *id);
void nnfeature_free(NNFeature * nfeature);

#endif /* NNFEATURE_H_ */
