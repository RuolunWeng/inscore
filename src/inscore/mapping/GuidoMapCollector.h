/*

  INScore Project

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


#ifndef __GuidoMapCollector__
#define __GuidoMapCollector__

#include <vector>
#include <string>

#include "GUIDOScoreMap.h"

class QGuidoGraphicsItem;

namespace inscore
{

/*!
\addtogroup ITLMapping Ressources mapping
@{
*/
//----------------------------------------------------------------------
/*!
	\brief a class to collect guido graphic maps
*/
class GuidoMapCollector: public MapCollector
{
	public :
//		typedef std::map<RelativeTimeSegment, GraphicSegment> Time2GraphicMap;
		typedef std::vector<std::pair<RelativeTimeSegment, GraphicSegment> > Time2GraphicMap;
		struct Filter {
			virtual ~Filter() {}
			virtual bool operator() (const GuidoElementInfos& ) const { return true; }
		} ;

	protected:
		const QGuidoGraphicsItem *	fItem;
		GuidoElementSelector	fSelector;
		const Filter*			fFilter;
		Time2GraphicMap*		fOutMap;		
		QPointF					fCurrentPageOrigin;
		
		void	CopyMap (const ::Time2GraphicMap& map);

	public :
				 GuidoMapCollector(const QGuidoGraphicsItem* item, GuidoElementSelector selector, const Filter* filter=0) 
					: fItem(item), fSelector(selector), fFilter(filter), fOutMap(0) {}
		virtual ~GuidoMapCollector() {}

		///< the method called by guido for each graphic segment
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );
		virtual void process (Time2GraphicMap* outmap);
				
		static RelativeTimeSegment	relativeTimeSegment(const TimeSegment& dates)
			{ return RelativeTimeSegment( libmapping::rational(dates.first.num , dates.first.denom) , libmapping::rational(dates.second.num , dates.second.denom) ); }
		static GraphicSegment		graphicSegment(const FloatRect& box, QPointF offset = QPointF(0,0))
			{ return GraphicSegment( box.left + offset.x(), box.top + offset.y(), box.right + offset.x(), box.bottom + offset.y()); }
};

//----------------------------------------------------------------------
/*!
	\brief a guido map collector filtering voice events
*/
class GuidoVoiceCollector: public GuidoMapCollector
{
	private:
		int fVoiceNum;
		struct AcceptVoicePredicat : public Filter { int fNum;
			virtual bool operator() (const GuidoElementInfos& infos) const { return infos.voiceNum == fNum; }
		} fVoiceFilter;
		void	setFilter(int num)			{ fVoiceFilter.fNum = num; fFilter = &fVoiceFilter; }

	public :
				 GuidoVoiceCollector(const QGuidoGraphicsItem* item, int num) 
					: GuidoMapCollector(item, kGuidoEvent), fVoiceNum(num) { if (num) setFilter(num); }
		virtual ~GuidoVoiceCollector() {}

		virtual void process (Time2GraphicMap* outmap);
};


//----------------------------------------------------------------------
/*!
	\brief a guido map collector that combines a guido events and staff mappings
*/
class GuidoStaffCollector: public GuidoMapCollector
{
	private:
		int	fStaffNum;

	public :
				 GuidoStaffCollector (const QGuidoGraphicsItem* item, int num) 
					: GuidoMapCollector(item, kGuidoStaff), fStaffNum(num) {}
		virtual ~GuidoStaffCollector()	{}

		virtual void process (Time2GraphicMap* outmap);
};

//----------------------------------------------------------------------
/*!
	\brief a guido map collector adjusting system to to slices start
*/
class GuidoSystemCollector: public GuidoMapCollector
{
	bool fFlatMode;
	
	public :
				 GuidoSystemCollector(const QGuidoGraphicsItem* item, bool flat=false) 
					: GuidoMapCollector(item, kGuidoSystem), fFlatMode(flat) { }
		virtual ~GuidoSystemCollector() {}

		virtual void processNoDiv (Time2GraphicMap* outmap);
		virtual void process (Time2GraphicMap* outmap);
};

/*!@} */

} // end namespoace

#endif
