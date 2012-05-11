/*
 * Copyright 2012 Carmen Alvarez
 *
 * This file is part of Nounours.
 *
 * Nounours is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Nounours is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nounours.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <pthread.h>
#include <string.h>
#include "nnclientmessage.h"

pthread_mutex_t nnclientmessage_mutex;

void nnclientmessage_setup() {
	pthread_mutex_init(&nnclientmessage_mutex, 0);
}
void nnclientmessage_init(XClientMessageEvent *event, NNUINounoursApp *uiapp,
		Atom atom, int format) {
	memset(event, 0, sizeof(XClientMessageEvent));
	event->type = ClientMessage;
	event->window = uiapp->window;
	event->format = format; // doesn't really matter since we use memcpy to pass the pointer
	event->message_type = atom;
}
void nnclientmessage_send(XClientMessageEvent *event, NNUINounoursApp *uiapp) {
	long event_mask = NoEventMask;
	if (uiapp->app->config.is_in_screensaver_mode)
		event_mask = ExposureMask; // TODO other applications may receive this event!
	pthread_mutex_lock(&nnclientmessage_mutex);
	XSendEvent(uiapp->background_display, uiapp->window, 0, event_mask,
			(XEvent*) event);
	XFlush(uiapp->background_display);
	pthread_mutex_unlock(&nnclientmessage_mutex);
}

void nnclientmessage_write(XClientMessageEvent *event, NNNounours *nounours,
		NNUIImage *uiimage) {
	void *ptr = event->data.l;
	memcpy(ptr, &uiimage, sizeof(NNUIImage*));
	ptr += sizeof(NNUIImage*);
	memcpy(ptr, &nounours, sizeof(NNNounours*));
}

void nnclientmessage_read(XClientMessageEvent *event, NNNounours **nounours,
		NNUIImage **uiimage) {
	void *ptr = event->data.l;
	memcpy(uiimage, ptr, sizeof(NNUIImage*));
	ptr += sizeof(NNUIImage*);
	memcpy(nounours, ptr, sizeof(NNNounours*));
}
