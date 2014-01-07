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



#include <Application.h>
#include <Window.h>
#include <math.h>
#include "main.h"
#include "ConwayWindow.h"
#include "BoardView.h"
#include "DeltaView.h"
#include "ControlView.h"

/*
	ConwayWindow()
	A constructor just to call the parent class.
*/
ConwayWindow::ConwayWindow(BRect frame)
				: BWindow(frame, "Conway", B_TITLED_WINDOW,B_NOT_RESIZABLE|B_NOT_ZOOMABLE)
{
}



/*
	MessageReceived()
	Forward messages to the application.
*/
void ConwayWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case mABOUT:
			be_app->PostMessage(B_ABOUT_REQUESTED);
			break;
		case mQUIT:
			be_app->PostMessage(B_QUIT_REQUESTED);
			break;
		case mRESET:
			// Fall through
		case mSTART_STOP:
			be_app->PostMessage(message->what);
			break;
		default:
			BWindow::MessageReceived(message);
			break;
	}
}


/*
	QuitRequested()
	Send the quit message to the application if the close box is clicked.
*/
bool ConwayWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}

