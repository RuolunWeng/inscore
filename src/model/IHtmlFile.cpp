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

#include "IHtmlFile.h"
#include "IMessage.h"
#include "TComposition.h"
#include "Updater.h"

using namespace std;

namespace INScore
{

//--------------------------------------------------------------------------
const string IHtmlFile::kHtmlFileType("htmlf");

//--------------------------------------------------------------------------
IHtmlFile::IHtmlFile( const std::string& name, IObject * parent )
	: IHtml(name, parent) 
{ 
	fTypeString = kHtmlFileType;
	fGetMsgHandlerMap[""] = TGetParamMsgHandler<string>::create(getFile());
}

//--------------------------------------------------------------------------
void IHtmlFile::accept (Updater* u)
{
	u->updateTo (SIHtmlFile(this));
}

//--------------------------------------------------------------------------
void IHtmlFile::print (ostream& out) const
{
	IHtml::print(out);
	TFile::print(out);
}

//--------------------------------------------------------------------------
MsgHandler::msgStatus IHtmlFile::set (const IMessage* msg )
{ 
	MsgHandler::msgStatus status = IObject::set(msg);
	if (status & (MsgHandler::kProcessed + MsgHandler::kProcessedNoChange)) return status; 

	status = TFile::set( msg );
	if (status & MsgHandler::kProcessed)
		if (!read(fText))
			status = MsgHandler::kCreateFailure;
		else newData(true);
	return status;
}

}
