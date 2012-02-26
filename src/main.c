/*
 * main.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "nnnounours.h"
#include "nnimage.h"
#include "nnfeature.h"
#include "nnanimation.h"
#include "nntheme.h"
#include "nncsvreader.h"
#include "nnpropertiesreader.h"

void help(char *prog_name) {
	printf("Usage:\n");
	printf("%s [-theme <path/to/theme>] [-screensaver]\n", prog_name);
	exit(1);
}
int main(int argc, char **argv) {
	const char *theme_path = "data/themes/1";
	int screensaver_mode = 0;

	int i;
	for(i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "-theme")) {
			if(i == argc) {
				help(argv[0]);
			} else {
				theme_path = argv[++i];
			}
		} else if(!strcmp(argv[i], "-screensaver")) {
			screensaver_mode = 1;
		} else {
			help(argv[0]);
		}
	}
	srandom(time(NULL));


	NNNounours *nounours = nnnounours_new(screensaver_mode);
	NNTheme *theme = nntheme_new(nounours, strdup(theme_path));
	nnnounours_use_theme(nounours, theme);
	while(1) {
		sleep(1000000);
	}
	return 0;
}
