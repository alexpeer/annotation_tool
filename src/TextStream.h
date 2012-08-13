//    Copyright Alex Peer 2012.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TEXTSTREAM_IS_FULL_OF_WELL_READ_FISH
#define TEXTSTREAM_IS_FULL_OF_WELL_READ_FISH

#include "EventStream.h"
#include "EventList.h"
#include <list>
#include <string>

#include "TextArea.h"

class Event_ShowText : public Event
{
public:
	std::string words;
	EventStream * stream;
	//TODO: needs link to owner, if sub-event?

	TextEvent_Drawable drawable;

	Event_ShowText( EventStream * stream = NULL, char * text = "", double when = 0 )
	{
		init( stream, text, when );
	}

	void init( EventStream * stream, char * text, double when )
	{
		this->stream = stream;
		words = text;
		this->when = when;
	}

	void activate()
	{
		printf( "Activate TextShow Event: %s\n", words.c_str() );
		drawable.setLabel( stream->name.c_str() );
		drawable.setContent( words.c_str() );
		stream->displayTarget->add( &drawable );
	}

	void deactivate()
	{
		printf( "Deactivate TextShow Event: %s\n", words.c_str() );
		//stream->displayTarget->remove( &drawable );
		drawable.removeFromParent();
	}
};

class Event_TextAnnotation : public EventWithDuration
{
public:
	
	EventStream * stream;
	Event_ShowText start;
	EventInverse stop;
	

	Event_TextAnnotation( double startTime = 0, double stopTime = 0, char* text = "", EventStream * eventStream = NULL )
	{
		this->stream = eventStream;

		startEvent = &start;
		stopEvent = &stop;

		start.when = startTime;
		stop.makeInverseOf( &start );
		stop.when = stopTime;

		start.stream = eventStream;

		start.words = text;
	}

	void Event::activate()		{ start.activate();	}

	void Event::deactivate()	{ stop.activate();	}

};

#endif