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

#ifndef NNCLIENTMESSAGE_H_
#define NNCLIENTMESSAGE_H_

#include <X11/Xlib.h>
#include "nnuinounoursapp.h"
#include "nnnounours.h"
#include "nnuiimage.h"

void nnclientmessage_setup();
void nnclientmessage_init(XClientMessageEvent *event, NNUINounoursApp *uiapp,
		Atom atom, int format);
void nnclientmessage_send(XClientMessageEvent *event, NNUINounoursApp *uiapp);
void nnclientmessage_write(XClientMessageEvent *event,
		struct NNNounours *nounours, struct NNUIImage *uiimage);
void nnclientmessage_read(XClientMessageEvent *event,
		struct NNNounours **nounours, struct NNUIImage **uiimage);

#endif /* NNCLIENTMESSAGE_H_ */
