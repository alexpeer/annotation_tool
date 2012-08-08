#ifndef EVENTSTREAM_HOLDS_THE_FIRE_AND_DREAMS_OF_OTHERS
#define EVENTSTREAM_HOLDS_THE_FIRE_AND_DREAMS_OF_OTHERS

#include <list>

// event stream organizes events by time, and can be navigated like a timeline
//  when moving through the timeline, events are triggered

class Event;

class EventStream : Stream
{
public:

	std::list<Event*> events;
	//std::list<TextEvent*> activeEvents;

	std::list<Event*>::iterator curr_pos;

	bool isPlaying;

	double cur_pos_milliseconds;

	EventStream();

	void seek( double milliseconds );

};

class Event
{
public:

	virtual void addTo( EventStream &stream );
	virtual void removeFrom( EventStream &stream );

	virtual void activate() = 0;
	virtual void deactivate() = 0;
	// activate and deactivate are expected to be inverse actions

	//virtual bool isActiveAtTime( double milliseconds ) = 0;	
	
	// need to overload < so lists can sort Events
	bool operator<( Event &rightArg )
	{
		return (this->when < rightArg.when);
	}

	bool needsReversal;
	Event * cancelsThis;
	// events can cancel others; used to for faster seek

	double when; // in milliseconds
};

class EventInverse : public Event
{
public:

	Event * inverseOf;

	EventInverse()
	{
		inverseOf = NULL;
	}

	EventInverse( Event * inverseOf )
	{
		makeInverseOf( inverseOf );
	}

	void makeInverseOf( Event * inverseOf )
	{
		this->inverseOf = inverseOf;
	}

	void activate()
	{
		inverseOf->deactivate();
	}

	void deactivate()
	{
		inverseOf->activate();
	}

};

// events can be instantaneous, or can have start and end durations; if they have durations, they'll have start and stop actions
class EventWithDuration : public Event
{
public:

	virtual void addTo( EventStream &stream );

	virtual void removeFrom( EventStream &stream );

	/*
	bool isActiveAtTime( double milliseconds )
	{
		if( milliseconds >= startEvent->when && milliseconds <= stop->when )
			return true;
		return false;
	}
	*/

	Event *startEvent;
	Event *stopEvent;
};

#endif