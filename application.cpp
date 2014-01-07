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
#include "application.h"
#include "math.h"

#include <Alert.h>
#include <Bitmap.h>
#include <stdlib.h>
#include <stdio.h>


/*
	ConwayApplication()
	Constructor: initialize data, build window.
*/

ConwayApplication::ConwayApplication()
		  		  : BApplication("application/x-vnd.ACConway")
{
	// Create bitmaps for both display and computation.
	board = new BBitmap(BRect(0, 0, VIEW_HSIZE - 1, VIEW_VSIZE - 1), B_COLOR_8_BIT);
	saveBoard = new BBitmap(BRect(0, 0, VIEW_HSIZE - 1, VIEW_VSIZE - 1), B_COLOR_8_BIT);
	deltas = new BBitmap(BRect(0, 0, VIEW_HSIZE - 1, VIEW_VSIZE - 1), B_COLOR_8_BIT);
	
	// Create the window and the views in it.
	theWindow = new ConwayWindow(BRect(10,40, WINDOW_HSIZE + 10, WINDOW_VSIZE + 40));

	theControlView = new ControlView(BRect(VIEW_HSIZE, 0,
									       VIEW_HSIZE + SPACE_BETWEEN_VIEWS,
									       VIEW_VSIZE), "Controls");
	theBoardView = new BoardView(BRect(0,0,VIEW_HSIZE, VIEW_VSIZE), "Board");
	theDeltaView = new DeltaView(BRect(VIEW_HSIZE + SPACE_BETWEEN_VIEWS, 0,
									   VIEW_HSIZE * 2 + SPACE_BETWEEN_VIEWS,
									   VIEW_VSIZE), "Delta");
	
	// Add views to window.
	theWindow->AddChild(theBoardView);
	theWindow->AddChild(theDeltaView);
	theWindow->AddChild(theControlView);
	
	// Make window visible.
	theWindow->Show();

	// Start the fun.
	Go();
}



/*
	~ConwayApplication()
	Destructor: deallocate dynamic memory on quit.
*/
ConwayApplication::~ConwayApplication()
{
	delete board;
	delete saveBoard;
	delete deltas;
	// Views and windows allocated in the constructor are deleted automatically when
	// the application quits.
}


/*
	QuitRequested()
	Standard message handler for the B_QUIT_REQUESTED message. 
	Clean up and print the statistics.
*/
bool ConwayApplication::QuitRequested(void)
{
	kill_thread(workerThread);
	PrintStats();
	return BApplication::QuitRequested();
}


/*
	AboutRequested()
	Standard message handler for the B_ABOUT_REQUESTED message. 
*/
void ConwayApplication::AboutRequested(void)
{
	AlertOK("Conway\n\nby Alex Chamberlain, chambera@cs.pdx.edu\n\n"
			"Conway comes with ABSOLUTELY NO WARRANTY.  This is free software,  "
			"and you are welcome to redistribute it under certain conditions.  "
			"See the file \"COPYING\" for details.");
}


/*
	MessageReceived()
	Standard message dispatcher to handle the messages from the buttons
		in the ControlView.
*/
void ConwayApplication::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case mRESET:
			kill_thread(workerThread);
			Go();
			break;
		case mSTART_STOP:
			if (going)
			{
				suspend_thread(workerThread);
				theControlView->SetStopButton("Go");
				going = false;
			}
			else
			{
				resume_thread(workerThread);
				theControlView->SetStopButton("Stop");
				going = true;
			}
			break;
		default:
			BApplication::MessageReceived(message);
	}
}



/*
	Go()
	Clean up and start a new game.
*/
void ConwayApplication::Go(void)
{
	PrintStats();
	InitBitmaps();
	InitStats();
	theWindow->Lock();
	theBoardView->DrawBitmap(board);
	theDeltaView->DrawBitmap(deltas);
	theControlView->SetStopButton("Stop");
	theControlView->EnableStopButton();
	theWindow->Unlock();

	going = true;
	workerThread = spawn_thread(_ProcessBoard, "Conway worker thread",
								B_DISPLAY_PRIORITY, static_cast<void*>(this));
	resume_thread(workerThread);
}


/*
	InitBitmaps()
	Initialize the bitmaps to the state they should be at the start of a game.
	   The delta display is completely black, meaning no changes in any cell, and
	   the board is randomly live/dead depending on the constant LIVE_FRACTION. 
*/
void ConwayApplication::InitBitmaps(void)
{
	// Initialize the random number generator in the usual UNIX way.
	time_t seed;
	time(&seed);
    srand(seed);

	for (int i = 0; i < VIEW_HSIZE; ++i)
	{
		for (int j = 0; j < VIEW_VSIZE; ++j)
		{
			setPixel(deltas, i, j, BLACK);
			setPixel(board, i, j, ((rand()/(double)RAND_MAX) < LIVE_FRACTION ?
										  LIVE_CELL_COLOR : DEAD_CELL_COLOR));
		}
	}
}


/*
	_ProcessBoard()
	Thread callbacks have to be static, but we want the worker thread to access class
		data, so this one just calls the real non-static worker function, ProcessBoard().
*/
int32 ConwayApplication::_ProcessBoard(void *theApp)
{
	return ((static_cast<ConwayApplication*>(theApp))->ProcessBoard());
}


/*
	ProcessBoard()
	Play the game after being started by the application.  This thread is not
		self-terminating---it depends on the caller to kill it and restart it
		when appropriate.
*/
int32 ConwayApplication::ProcessBoard(void)
{
	int i, j, n;
	uint8 p;
	
	// Update forever.
	while(1)
	{
		births = 0;
		deaths = 0;
		// Update most recent births and deaths and save current state of board.
		for (i = 0; i <= VIEW_HSIZE - 1; ++i)
			for (j = 0; j <= VIEW_VSIZE - 1; ++j)
			{
				p = getPixel(board, i, j);
				if (p == NEWBORN_CELL_COLOR)
				{
					setPixel(board, i, j, LIVE_CELL_COLOR);
					setPixel(saveBoard, i, j, LIVE_CELL_COLOR);
				}
				else if (p == DYING_CELL_COLOR)
				{
					setPixel(board, i, j, DEAD_CELL_COLOR);
					setPixel(saveBoard, i, j, DEAD_CELL_COLOR);
				}
				else
					setPixel(saveBoard, i, j, p);
			}
				
		// Now compute the number of neighbors for each cell and call ProcessCell()
		// to update it.
		
		// Top left corner
		n = 0;
		p = getPixel(saveBoard, 0, 1);
		if (p == LIVE_CELL_COLOR)
			++n;
		p = getPixel(saveBoard, 1, 1);
		if (p == LIVE_CELL_COLOR)
			++n;
		p = getPixel(saveBoard, 1, 0);
		if (p == LIVE_CELL_COLOR)
			++n;
		ProcessCell(0, 0, n);
	
		// Top right corner
		n = 0;
		p = getPixel(saveBoard, VIEW_HSIZE - 1, 1);
		if (p == LIVE_CELL_COLOR)
			++n;
		p = getPixel(saveBoard, VIEW_HSIZE - 2, 1);
		if (p == LIVE_CELL_COLOR)
			++n;
		p = getPixel(saveBoard, VIEW_HSIZE - 2, 0);
		if (p == LIVE_CELL_COLOR)
			++n;
		ProcessCell(VIEW_HSIZE - 1, 0, n);
		
		// Bottom left corner
		n = 0;
		p = getPixel(saveBoard, 0, VIEW_VSIZE - 2);
		if (p == LIVE_CELL_COLOR)
			++n;
		p = getPixel(saveBoard, 1, VIEW_VSIZE - 2);
		if (p == LIVE_CELL_COLOR)
			++n;
		p = getPixel(saveBoard, 1, VIEW_VSIZE - 1);
		if (p == LIVE_CELL_COLOR)
			++n;
		ProcessCell(0, VIEW_VSIZE - 1, n);
		
		// Bottom right corner
		n = 0;
		p = getPixel(saveBoard, VIEW_HSIZE - 1, VIEW_VSIZE - 2);
		if (p == LIVE_CELL_COLOR)
			++n;
		p = getPixel(saveBoard, VIEW_HSIZE - 2, VIEW_VSIZE - 2);
		if (p == LIVE_CELL_COLOR)
			++n;
		p = getPixel(saveBoard, VIEW_HSIZE - 2, VIEW_VSIZE - 1);
		if (p == LIVE_CELL_COLOR)
			++n;
		ProcessCell(VIEW_HSIZE - 1, VIEW_VSIZE - 1, n);
	
		// Top edge
		for (i = 1; i <= VIEW_HSIZE - 2; ++i)
		{
			n = 0;
			p = getPixel(saveBoard, i - 1, 0);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, i + 1, 0);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, i - 1, 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, i + 1, 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, i, 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			ProcessCell(i, 0, n);
		}
	
		// Bottom edge
		for (i = 1; i <= VIEW_HSIZE - 2; ++i)
		{
			n = 0;
			p = getPixel(saveBoard, i - 1, VIEW_VSIZE - 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, i + 1, VIEW_VSIZE - 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, i - 1, VIEW_VSIZE - 2);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, i + 1, VIEW_VSIZE - 2);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, i, VIEW_VSIZE - 2);
			if (p == LIVE_CELL_COLOR)
				++n;
			ProcessCell(i, VIEW_VSIZE - 1, n);
		}
	
		// Left edge
		for (j = 1; j <= VIEW_VSIZE - 2; ++j)
		{
			n = 0;
			p = getPixel(saveBoard, 0, j - 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, 0, j + 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, 1, j - 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, 1, j + 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, 1, j);
			if (p == LIVE_CELL_COLOR)
				++n;
			ProcessCell(0, j, n);
		}
	
		// Right edge
		for (j = 1; j <= VIEW_VSIZE - 2; ++j)
		{
			n = 0;
			p = getPixel(saveBoard, VIEW_HSIZE - 1, j - 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, VIEW_HSIZE - 1, j + 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, VIEW_HSIZE - 2, j - 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, VIEW_HSIZE - 2, j + 1);
			if (p == LIVE_CELL_COLOR)
				++n;
			p = getPixel(saveBoard, VIEW_HSIZE - 2, j);
			if (p == LIVE_CELL_COLOR)
				++n;
			ProcessCell(VIEW_HSIZE - 1, j, n);
		}
	
		// Remainder of board
		for (i = 1; i <= VIEW_HSIZE - 2; ++i)
		{
			for (j = 1; j <= VIEW_VSIZE - 2; ++j)
			{
				n = 0;
				p = getPixel(saveBoard, i - 1, j - 1);
				if (p == LIVE_CELL_COLOR)
					++n;
				p =getPixel(saveBoard, i, j - 1);
				if (p == LIVE_CELL_COLOR)
					++n;
				p =getPixel(saveBoard, i + 1, j - 1);
				if (p == LIVE_CELL_COLOR || p == DYING_CELL_COLOR)
					++n;
				p =getPixel(saveBoard, i - 1, j);
				if (p == LIVE_CELL_COLOR || p == DYING_CELL_COLOR)
					++n;
				p =getPixel(saveBoard, i + 1, j);
				if (p == LIVE_CELL_COLOR || p == DYING_CELL_COLOR)
					++n;
				p =getPixel(saveBoard, i - 1, j + 1);
				if (p == LIVE_CELL_COLOR || p == DYING_CELL_COLOR)
					++n;
				p =getPixel(saveBoard, i, j + 1);
				if (p == LIVE_CELL_COLOR || p == DYING_CELL_COLOR)
					++n;
				p =getPixel(saveBoard, i + 1, j + 1);
				if (p == LIVE_CELL_COLOR || p == DYING_CELL_COLOR)
					++n;
				ProcessCell(i, j, n);
			}
		}
		// We're done updating the bitmaps, so draw them into the window
		//    and update the statistics.
		theWindow->Lock();
		theBoardView->DrawBitmapAsync(board);
		theDeltaView->DrawBitmapAsync(deltas);
		theBoardView->Sync();
		theWindow->Unlock();
		++stats.numBirthsOfSize[births];
		++stats.numDeathsOfSize[deaths];
	}
	return 0;	// Meaningless since execution will never reach this point---the
				//    thread will always be killed in the while loop.
}		
	
			
void ConwayApplication::ProcessCell(int x, int y, int neighbors)
{
	uint8 newCell;
	uint8 oldCell = getPixel(saveBoard, x, y);

	if (neighbors <= 1)
	{
		if (oldCell == DEAD_CELL_COLOR)
			newCell = oldCell;
		else
		{
			newCell = DYING_CELL_COLOR;
			++deaths;
		}
	}
	else if (neighbors == 2)
	{
		newCell = oldCell;
	}
	else if (neighbors == 3)
	{
		if (oldCell == LIVE_CELL_COLOR)
			newCell = LIVE_CELL_COLOR;
		else
		{
			newCell = NEWBORN_CELL_COLOR;
			++births;
		}
	}
	else	// neighbors >= 4
	{
		if (oldCell == DEAD_CELL_COLOR)
			newCell = oldCell;
		else
		{
			newCell = DYING_CELL_COLOR;
			++deaths;
		}
	}

	uint8 oldDelta = getPixel(deltas, x, y);
	uint8 newDelta;
	if (newCell != oldCell)
	{
		setPixel(board, x, y, newCell);
		newDelta = WHITE;
	}
	else	// newCell == oldCell
		newDelta = (oldDelta == BLACK ? BLACK : oldDelta - 1);

	if (newDelta != oldDelta)
		setPixel(deltas, x, y, newDelta);
}


/*
	PrintStats()
	Print out a table of size/frequency pairs.
*/
void ConwayApplication::PrintStats(void)
{
	printf("\n");
	for (int size = 1; size <= VIEW_HSIZE * VIEW_VSIZE; ++size)
	{
		int changes = stats.numBirthsOfSize[size] + stats.numDeathsOfSize[size];
		if (changes)
			printf("%i\t%i\n", size, changes);
	}
}


/*
	InitStats()
	Clear the statistics in preparation for a new game.
*/
void ConwayApplication::InitStats(void)
{
	for (int size = 1; size <= VIEW_HSIZE * VIEW_VSIZE; ++size)
	{
		stats.numBirthsOfSize[size] = 0;
		stats.numDeathsOfSize[size] = 0;
	}
}


/*
	AlertOK()
	Show an alert box with the given message.
*/
void ConwayApplication::AlertOK(char* message)
{
	BAlert	*a = new BAlert("", message, "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
	a->SetShortcut(0, B_ENTER);
	a->Go();
}

