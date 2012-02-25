/*
 * nnnounours.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include "nnnounours.h"
#include "nntheme.h"
#include "nnpropertiesreader.h"
#include "nnmath.h"

NNNounours * nnnounours_new() {
	NNNounours *nounours = malloc(sizeof(NNNounours));
	nounours->is_doing_animation = 0;
	nounours->fling_factor = 0;
	nounours->shake_factor = 0;
	nounours->fling_precision = 0;
	nounours->idle_time = 0;
	nounours->idle_ping_interval = 0;
	nounours->uinounours = nnuinounours_new(nounours);
	nounours->cur_theme = 0;
	nounours->cur_image = 0;
	nounours->cur_feature = 0;
	nounours->last_x = -1;
	nounours->last_y = -1;
	nounours->last_motion_event_time_us = 0;
	nnread_nounours_properties_file(nounours);
	nnuinounours_start_loop(nounours->uinounours);

	return nounours;
}
void nnnounours_use_theme(NNNounours *nounours, NNTheme *theme) {
	nounours->cur_theme = theme;
	nnuinounours_resize(nounours->uinounours, theme->width, theme->height);
	nnnounours_show_image(nounours, theme->default_image);
}

void nnnounours_show_image(NNNounours *nounours, NNImage *image) {
	if (nounours->cur_image == image)
		return;
	nounours->cur_image = image;
	nnuinounours_show_image(nounours->uinounours, image->uiimage);
}
void nnnounours_show_animation(NNNounours *nounours, NNAnimation *animation) {
	if (nounours->is_doing_animation)
		return;
	nounours->is_doing_animation = 1;
	nnanimation_show(animation);
	nounours->is_doing_animation = 0;
}

void nnnounours_on_press(NNNounours *nounours, int x, int y) {
	if (nounours->is_doing_animation)
		return;
	nounours->cur_feature = nnimage_find_closest_feature(nounours->cur_image, x,
			y);
	if (nounours->cur_feature != 0) {
		NNAdjacentImages *adjacent_images = nnimage_find_adjacent_images(
				nounours->cur_image, nounours->cur_feature);
		if (adjacent_images == 0) {
			nounours->cur_image = nounours->cur_theme->default_image;
		}
	}
}
void nnnounours_on_move(NNNounours *nounours, int x, int y) {
	if (nounours->is_doing_animation)
		return;

	NNImage *image = nnimage_find_adjacent_image(nounours->cur_image,
			nounours->cur_feature, x, y);
	nnnounours_show_image(nounours, image);
	struct timeval now_tv;
	gettimeofday(&now_tv, NULL);
	long now_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;
	if (nounours->last_x >= 0) {
		long time_diff = now_us - nounours->last_motion_event_time_us;
		float vel_x = 1000000 * (x - nounours->last_x) / time_diff;
		float vel_y = 1000000 * (y - nounours->last_y) / time_diff;
		nnnounours_on_fling(nounours, x, y, vel_x, vel_y);
	}
	nounours->last_motion_event_time_us = now_us;
	nounours->last_x = x;
	nounours->last_y = y;
}
void nnnounours_on_release(NNNounours *nounours, int x, int y) {
	if (nounours->is_doing_animation)
		return;

	if (nounours->cur_image->release != 0)
		nnnounours_show_image(nounours, nounours->cur_image->release);
}
void nnnounours_on_fling(NNNounours *nounours, int x, int y, float vel_x,
		float vel_y) {
	printf("fling %dx%d, %f %f\n", x, y, vel_x, vel_y);
	int i;
	NNTheme *theme = nounours->cur_theme;
	for (i = 0; i < theme->num_animation_flings; i++) {
		NNAnimationFling *fling = theme->animation_flings[i];
		if (!nnmath_is_faster(vel_x, fling->min_vel_x))
			continue;
		if (!nnmath_is_faster(vel_y, fling->min_vel_y))
			continue;
		if (!nnmath_point_is_in_square(x, y, fling->x, fling->y, fling->width,
				fling->height))
			continue;
		nnnounours_show_animation(nounours, fling->animation);
	}
}

void nnnounours_free(NNNounours *nounours) {
	free(nounours);
}
