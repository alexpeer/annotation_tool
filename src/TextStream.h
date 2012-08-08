#ifndef TEXTSTREAM_IS_FULL_OF_WELL_READ_FISH
#define TEXTSTREAM_IS_FULL_OF_WELL_READ_FISH

#include "Stream.h"
#include "EventList.h"
#include <list>
#include <string>

#include "TextArea.h"

class Event_ShowText : public Event
{
public:
	std::string words;
	Stream * parent;

	DrawList * target;

	TextEvent_Drawable drawable;

	void activate()
	{
		target->add( drawable );
	}

	void deactivate()
	{
		drawable.remove();
	}
};

class Event_TextAnnotation : public EventWithDuration
{
public:

	//TODO: now needs draw target
	DrawList * drawTarget;
	Event_ShowText start;
	EventInverse stop;
	

	Event_TextAnnotation( double startTime = 0, double stopTime = 0, char* text = "" )
	{
		
		startEvent = &start;
		stopEvent = &stop;

		start.when = startTime;
		stop.makeInverseOf( &start );
		stop.when = stopTime;

		start.words = text;
	}

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