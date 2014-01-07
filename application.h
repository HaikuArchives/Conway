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

class ConwayApplication : public BApplication
{
	public:
		ConwayApplication();
		~ConwayApplication();
		bool QuitRequested(void);
		void AboutRequested(void);
		void MessageReceived(BMessage *message);

	private:
		static int32 _ProcessBoard(void *theApp);
		int32 ProcessBoard(void);
		void ProcessCell(int x, int y, int neighbors);
		void InitBitmaps(void);
		void PrintStats(void);
		void InitStats(void);
		void Go(void);
		void AlertOK(char *message);
		
		bool going;
		int births, deaths;
		struct
		{
			int numBirthsOfSize[VIEW_HSIZE * VIEW_VSIZE + 1];
			int numDeathsOfSize[VIEW_HSIZE * VIEW_VSIZE + 1];
		} stats;
};

