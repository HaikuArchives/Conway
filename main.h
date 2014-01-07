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



#pragma once


#include "ConwayWindow.h"
#include "BoardView.h"
#include "DeltaView.h"
#include "ControlView.h"

// Size of board.
#define VIEW_HSIZE			300
#define VIEW_VSIZE			300

// Layout constants.
#define SPACE_BETWEEN_VIEWS	70
#define WINDOW_HSIZE		(VIEW_HSIZE * 2 + SPACE_BETWEEN_VIEWS)
#define WINDOW_VSIZE		(VIEW_VSIZE)

// Colors for the bitmaps.
#define BLACK 0
#define WHITE 31
#define DEAD_CELL_COLOR		BLACK
#define DYING_CELL_COLOR	43	// Bright red
#define NEWBORN_CELL_COLOR	32	// Bright blue
#define LIVE_CELL_COLOR		WHITE

// Fraction of cells to make live when initializing the board.
#define LIVE_FRACTION .5

// Application-specific BMessages.
enum
{
	mABOUT				= 'ALab',	// About box selected.
	mQUIT 				= 'ALqu',	// Initiate quit process.
	mRESET				= 'ALre',	// Reset board.
	mSTART_STOP			= 'ALst'	// Start/stop computations.
};

// Global variables make things faster.
extern BBitmap *board, *saveBoard, *deltas;
extern thread_id workerThread;
extern ConwayWindow *theWindow;
extern BoardView *theBoardView;
extern DeltaView *theDeltaView;
extern ControlView *theControlView;
	
// A couple of useful global functions.
void setPixel(BBitmap *b, int x, int y, uint8 color);
uint8 getPixel(BBitmap *b, int x, int y);

