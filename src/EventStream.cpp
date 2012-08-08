#include "EventStream.h"

// EventList
	bool EventList::loadFromFile( char* filename )
	{
		//TODO: this

		//at end, seek to 0 ( be sure current milliseconds is set to < 0 )

		return true;
	}
	
	void EventList::addEvent( Event * newEvent )
	{
		events.push_back( newEvent );
		newEvent->addTo( *parent );
	}

	//void EventList::addTo( EventStream stream )
	//{		
	//	std::list<Event*>::iterator it;
	//	for( it = events.begin(); it != events.end(); ++it )
	//	{
	//		stream.events.push_back( (*it) );
	//	}
	//}

// EventStream
	EventStream::EventStream()
	{
		type = TEXT;
		isPlaying = false;
		cur_pos_milliseconds = -1; //so that on initital seek to 0, any events at time 0 are activated

		eventList.parent = this;
	}

	void EventStream::seek( double milliseconds )
	{
		if( events.size() == 0 )
			return;
		
		if( needsSort )
		{
			sort();
		}

		if( cur_pos_milliseconds > milliseconds )
		// moving left (earlier)
		{
			// we want the event one before the desired position to be active
			// so, going left; go until we're at time or first before
			// going left, we deactivate events
			bool hasHitDesiredTime = false;
			while( !hasHitDesiredTime )
			{
				if( cur_pos_milliseconds > (*curr_pos)->when )
					(*curr_pos)->deactivate();

				// check if we're past where we want
				if( (*curr_pos)->when <= milliseconds )
				{	
					hasHitDesiredTime = true;
					//if( cur_pos_milliseconds > (*curr_pos)->when )
					(*curr_pos)->activate();
					break;
				}

				// check if at beginning of the list 
				// ( and here, that means desired pos is before beginning of list )
				if( curr_pos == events.begin() )
				{
					hasHitDesiredTime = true;
					break;
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
			std::list<Event*>::iterator next;
			bool hasHitDesiredTime = false;
			while( !hasHitDesiredTime )
			{
				if( cur_pos_milliseconds < (*curr_pos)->when
				&&  milliseconds >= (*curr_pos)->when )
					(*curr_pos)->activate();

				// if we're there (or at end of stream), stop
				if( (*curr_pos)->when == milliseconds)
				{	
					hasHitDesiredTime = true;
					break;
				}

				next = curr_pos;
				next++;
				
				// if next pos is end of stream, stop
				if( next == events.end() )
				{
					hasHitDesiredTime = true;
					break;					
				}

				// if next pos is > desired pos, stop
				if( (*next)->when > milliseconds )
				{
					hasHitDesiredTime = true;
					break;
				}

				curr_pos = next;
			}
		}

		cur_pos_milliseconds = milliseconds;

	}

	double EventStream::getDuration()
	{
		if( events.size() == 0 )
			return 0;
		return (*(events.end()))->when;
	}
		
	// functor for sort (ugh)
	struct EventPointerComparison {
		bool operator() (const Event* left, const Event* right) const { 
			return left->when < right->when;
		}
	};
	void EventStream::sort()
	{
		events.sort( EventPointerComparison() );
	   	   
		needsSort = false;
		curr_pos = events.begin();
    }

// Event

	void Event::addTo( EventStream &stream )
	{
		stream.internal_addEvent( this );
	}
	void Event::removeFrom( EventStream &stream )
	{
		stream.internal_removeEvent( this );
	}

// EventWithDuration

	void EventWithDuration::addTo( EventStream &stream )
	{
		//assume duration events cancel each other
		startEvent->cancelsThis = stopEvent;
		stopEvent->cancelsThis = startEvent;

		stream.internal_addEvent( startEvent );
		stream.internal_addEvent( stopEvent );
	}

	void EventWithDuration::removeFrom( EventStream &stream )
	{
		stream.internal_removeEvent( startEvent );
		stream.internal_removeEvent( stopEvent );
	}