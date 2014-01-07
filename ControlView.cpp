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



#include "main.h"

/*
	ControlView()
	Constructor: create the buttons and add them to the view so we don't have to worry about
		updating or deallocating them.
*/
ControlView::ControlView(BRect rect, char *name)
	   	   : BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW)
{
	// Create some buttons, centered in the view, and spaced evenly down from BUTTON_START.
	aboutButton = new BButton(BRect(5, BUTTON_START, SPACE_BETWEEN_VIEWS - 5,
									BUTTON_START + BUTTON_HEIGHT),
							 "About button", "About", new BMessage(mABOUT));
	stopButton = new BButton(BRect(5, BUTTON_START + BUTTON_HEIGHT + BUTTON_SPACE,
								   SPACE_BETWEEN_VIEWS - 5,
								   BUTTON_START + BUTTON_HEIGHT * 2 + BUTTON_SPACE),
							 "Stop button", "Stop", new BMessage(mSTART_STOP));
	resetButton = new BButton(BRect(5, BUTTON_START + BUTTON_HEIGHT * 2 + BUTTON_SPACE * 2,
									SPACE_BETWEEN_VIEWS - 5,
									BUTTON_START + BUTTON_HEIGHT * 3 + BUTTON_SPACE * 2),
							  "Reset button", "Reset", new BMessage(mRESET));

	AddChild(aboutButton);
	AddChild(stopButton);
	AddChild(resetButton);
}


/*
	AttachedToWindow()
	The standard place to set the background color for the view.
*/
void ControlView::AttachedToWindow()
{
//	SetHighColor(255,255,0); 
	SetViewColor(200,200,200);	// Medium-dark gray
}


/*
	SetStopButton()
	Set the button text for the "Stop" button to the given string.  Also lock the window
		beforehand so the caller doesn't have to.  (Nested locks are OK.)
*/
void ControlView::SetStopButton(char *label)
{
	Window()->Lock();
	stopButton->SetLabel(label);
	Window()->Unlock();
}


/*
	DisableStopButton()
	Gray out the "Stop" button.  Also lock the window beforehand so the caller doesn't have to.
*/
void ControlView::DisableStopButton(void)
{
	Window()->Lock();
	stopButton->SetEnabled(false);
	Window()->Unlock();
}


/*
	EnableStopButton()
	Un-gray the "Stop" button.  Also lock the window beforehand so the caller doesn't have to.
*/
void ControlView::EnableStopButton(void)
{
	Window()->Lock();
	stopButton->SetEnabled(true);
	Window()->Unlock();
}


