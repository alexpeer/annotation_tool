#ifndef TEXTSTREAM_IS_FULL_OF_WELL_READ_FISH
#define TEXTSTREAM_IS_FULL_OF_WELL_READ_FISH

#include "Stream.h"
#include <list>
#include <string>

class TextEvent
{
public:
	std::string words;
	
	// times in milliseconds
	double start;
	double stop;

	bool isActiveAtTime( double milliseconds )
	{
		if( milliseconds >= start && milliseconds <= stop )
			return true;
		return false;
	}
};

class TextStream : public Stream
{
public:

	std::list<TextEvent> events;
	//std::list<TextEvent*> activeEvents;

	std::list<TextEvent>::iterator curr_pos;

	bool isPlaying;

	double cur_pos_milliseconds;

	bool isActive; // do we have an even to display at the current play time

	TextStream()
	{
		type = TEXT;
		isPlaying = false;
		curr_pos = NULL;
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

		if(curr_pos == NULL)
			curr_pos = events.begin();
		
		std::list<TextEvent>::iterator next, prev;
		//float lastTime;

		if( curr_pos->start < milliseconds )
		// moving left
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
		cur_pos_milliseconds += milliseconds;

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



};

#endif