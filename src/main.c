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

	const char *theme_id = "1";
	if(argc > 1) {
		theme_id = argv[1];
	}
	NNNounours *nounours = nnnounours_new();
	NNTheme *theme = nntheme_new(nounours, theme_id);
	nnnounours_use_theme(nounours, theme);
	while(1) {
		sleep(1000000);
	}
	return 0;
}
