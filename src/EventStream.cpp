#include "EventStream.h"

// EventStream
	EventStream::EventStream()
	{
		type = TEXT;
		isPlaying = false;
		cur_pos_milliseconds = 0;
	}

	void EventStream::seek( double milliseconds )
	{
		if( events.size() == 0 )
			return;

		if( curr_pos->start > milliseconds )
		// moving left (earlier)
		{
			// we want the event one before the desired position to be active
			// so, going left; go until we're at time or first before
			// going left, we deactivate events
			bool hasHitDesiredTime = false;
			while( !hasHitDesiredTime )
			{
				(*curr_pos)->deactivate();

				if( (*curr_pos)->when <= milliseconds)
				{	
					hasHitDesiredTime = true;
					(*curr_pos)->activate();
				}

				curr_pos--;
			}
		}
		else
		// moving right (or, already there)
		{			
			// we want the event one before the desired position to be active
			// so, going right; go until we're at time but don't pass
			// going right, we activate events	
			std::list<TextEvent>::iterator next;
			bool hasHitDesiredTime = false;
			while( !hasHitDesiredTime )
			{
				// if we're there, stop
				if( (*curr_pos)->when == milliseconds )
				{	
					hasHitDesiredTime = true;
					break;
				}

				// if next pos is > desired pos, stop
				next = curr_pos;
				next++;

				if( next->when > milliseconds )
				{
					hasHitDesiredTime = true;
					break;
				}

				// if 
				(*curr_pos)->activate();

				if( (*curr_pos)->when <= milliseconds)
				{	
					hasHitDesiredTime = true;
					(*curr_pos)->activate();
				}

			}
		}

		cur_pos_milliseconds = milliseconds;

	}

// Event

	void Event::addTo( EventStream &stream )
	{
		stream.add( this );
	}
	void Event::removeFrom( EventStream &stream )
	{
		stream.remove( this );
	}

// EventWithDuration

	void EVentWithDuration::addTo( EventStream &stream )
	{
		//assume duration events cancel each other
		startEvent->cancelsThis = stopEvent;
		stopEvent->cancelsThis = startEvent;

		stream.add( startEvent );
		stream.add( stopEvent );
	}

	void EVentWithDuration::removeFrom( EventStream &stream )
	{
		stream.remove( startEvent );
		stream.remove( stopEvent );
	}