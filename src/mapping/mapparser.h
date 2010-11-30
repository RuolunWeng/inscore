/*
  Interlude Library
  Copyright (C) 2009,2010  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __mapparser__
#define __mapparser__

#include <stdio.h>

namespace inscore 
{

class mapreader;
/* \brief a class for reading mapping files or strings
*/
class mapparser {
	int parse (FILE *fd);
	int parse (const char *filename);
	void start();
	void stop();			
	
	mapreader* fReader;

	public:
				 mapparser(mapreader* reader) : fReader(reader) {}
		virtual ~mapparser() {};
		
		bool readstring (const char * buffer);
		bool readfile (FILE* fd);
		bool readfile (const char * file);
};

} // end namespace

#endif
