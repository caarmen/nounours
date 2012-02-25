/*
 * main.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdio.h>
#include "nnnounours.h"
#include "nnimage.h"
#include "nnfeature.h"
#include "nnanimation.h"
#include "nntheme.h"
#include "nncsvreader.h"
#include "nnpropertiesreader.h"
int main(int argc, char **argv) {

	NNNounours *nounours = nnnounours_new();
	NNTheme *theme = nntheme_new(nounours, "1");
	nnnounours_use_theme(nounours, theme);
	while(1) {
		sleep(1000000);
	}
	return 0;
}
