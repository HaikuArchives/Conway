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


#include <new>
#include "main.h"
#include "application.h"

// Global variables
BBitmap *board, *saveBoard, *deltas;	// Bitmaps for both computation and display.
thread_id workerThread;					// A separate thread to run the game.
ConwayWindow *theWindow;					// The main (only) window.
BoardView *theBoardView;				// Contains the board.
DeltaView *theDeltaView;				// Contains the delta display.
ControlView *theControlView;			// Contains the buttons.


/*
	main()
	Initialize and run the application.
*/
int main()
{	
	ConwayApplication *theApp = new(nothrow) ConwayApplication;

	if (theApp)
	{
		theApp->Run();
		delete theApp;
		return 0;
	}
	return 1;
}


/*
	setPixel()
	Set the color of the pixel at the given coordinates in the given BBitmap
		to the given color.  Only works with 8-bit bitmaps!
*/
void setPixel(BBitmap *b, int x, int y, uint8 color)
{
	uint8 *bits = (uint8*)(b->Bits());
	bits[x + y * VIEW_HSIZE] = color;
}


/*
	getPixel()
	Get the color of the pixel at the given coordinates in the given BBitmap.
		Only works with 8-bit bitmaps!
*/
uint8 getPixel(BBitmap *b, int x, int y)
{
	uint8 *bits = (uint8*)(b->Bits());
	return bits[x + y * VIEW_HSIZE];
}


