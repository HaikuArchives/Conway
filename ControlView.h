/*
    Copyright 1998, Alex Chamberlain

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef __CONTROLVIEW_H
#define __CONTROLVIEW_H


#include <View.h>
#include <Button.h>


#define NUM_BUTTONS 3
#define BUTTON_HEIGHT	15
#define BUTTON_SPACE	20
// Center the buttons.
#define BUTTON_START	(((WINDOW_VSIZE)-(BUTTON_HEIGHT*NUM_BUTTONS+BUTTON_SPACE*(NUM_BUTTONS-1)))/2-5)

class ControlView : public BView
{
	public:
		ControlView(BRect frame, char *name); 
		virtual	void AttachedToWindow();
		void EnableStopButton(void);
		void DisableStopButton(void);
		void SetStopButton(char *label);
	private:
		BButton *stopButton, *aboutButton, *resetButton;
};

#endif /* __CONTROLVIEW_H */
