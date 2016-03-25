/*
  Copyright � Grame 2008

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
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
    research@grame.fr

*/

#ifndef __etailOperation__
#define __etailOperation__

#include <map>
#include <string>

#include "tailOperation.h"

namespace guido 
{

/*!
\addtogroup operations
@{
*/

/*!
\brief A visitor that cuts the head of a score before a given event number.
*/
class gar_export etailOperation : public tailOperation
{		
    public:
 				 etailOperation()	{}
		virtual ~etailOperation()	{}

		/*! cuts the head of a score after a given duration
			\param score the score to be cut
			\param evIndex the number of events to cut
			\return a new score
		*/
		Sguidoelement operator() ( const Sguidoelement& score, unsigned int evIndex, unsigned int voiceIndex=0 );
 
		/*! cuts the head of a score after a given duration
			\param score1 the score to be cut
			\param score2 a score which events count is used as cut point. 
			\return a new score
		*/
		virtual SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 );

};

/*! @} */

} // namespace MusicXML


#endif
