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

/*
class TextStream : public Stream
{
public:

	std::list<TextEvent> events;
	//std::list<TextEvent*> activeEvents;

	std::list<TextEvent>::iterator curr_pos;

	bool isPlaying;

	double cur_pos_milliseconds;

	bool isActive; // do we have an event to display at the current play time

	TextStream()
	{
		type = TEXT;
		isPlaying = false;
		cur_pos_milliseconds = 0;
		isActive = false;
	}

	virtual void play()
	{
		isPlaying = true;
	}
	virtual void stop()
	{
		isPlaying = false;
	}

	//TODO: this stream nav stuff assumes events happen one at a time; need a fancier data structure for overlap
	void seek( double milliseconds )
	{
		if( events.size() == 0 )
			return;
		
		std::list<TextEvent>::iterator next, prev;
		//float lastTime;

		if( curr_pos->start < milliseconds )
		// moving left
		{
			// we want the event one before the desired position to be active
			// so, going left; go until we're at time or first before
			while(true)
			{
				if( (*curr_pos).when )
				{
					isActive = true;
					break;
				}
				if(curr_pos->start > milliseconds)
				{	isActive = false;
					break;
				}

				//lastTime = curr_pos->start;
				curr_pos--;
			}
		}
		else
		// moving right (or, already there)
		{			
			while(true)
			{
				if( (*curr_pos).isActiveAtTime( milliseconds ) )
				{
					isActive = true;
					break;
				}
				if(curr_pos->start > milliseconds)
				{	isActive = false;
					break;
				}

				//lastTime = curr_pos->start;
				curr_pos++;
			}
		}

		cur_pos_milliseconds = milliseconds;

	}
	
	void update( double elapsed_milliseconds )
	{
		cur_pos_milliseconds += elapsed_milliseconds;

		seek( cur_pos_milliseconds );
	}

	// all times should be in milliseconds
	double getDuration()
	{
		if( events.size() == 0 )
			return 0;
		
		return events.end()->stop;
	}
	double getCurrentPosition()
	{
		return cur_pos_milliseconds;
	}

	void addEvent( double start, double stop, char* text )
	{
		//TODO: update iterator?

		events.push_back( TextEvent( start, stop, text ) );

		//TODO: add in order, instead of sort
		events.sort();

		//TODO: also index, for binary search-style nav

		iteratorInit();
		seek( cur_pos_milliseconds );
	}

	void iteratorInit()
	{
		curr_pos = events.begin();
	}

};
*/

#endif