#ifndef EVENTSTREAM_HOLDS_THE_FIRE_AND_DREAMS_OF_OTHERS
#define EVENTSTREAM_HOLDS_THE_FIRE_AND_DREAMS_OF_OTHERS

#include <list>
#include "Stream.h"

// event stream organizes events by time, and can be navigated like a timeline
//  when moving through the timeline, events are triggered

class Event;
class EventStream;

class EventList
{
public:
// EventList loads it's events into the master EventStream, so most of it's Stream business is handled there
// however it'll pretend to be a Stream for now, so it can be stored / visualized with media streams ( TODO: decide how to make that cleaner )
	
	EventStream * parent;
	std::list<Event*> events;

	bool loadFromFile( char* filename );
	
	void addTo( EventStream stream );

	void addEvent( Event * newEvent );

	void deleteEvents()
	{
		std::list<Event*>::iterator it;
		for( it = events.begin(); it != events.end(); ++it )
		{
			delete *it;
		}
	}
	~EventList()
	{
		deleteEvents();
	}
};
class EventStream : public Stream
{
public:

// event storage and timeline stuff
	std::list<Event*> events;	// holds streamlined event execution info for seek
	EventList eventList;		// holds detailed event info

	bool needsSort;

	std::list<Event*>::iterator curr_pos;

	double cur_pos_milliseconds;

	EventStream();

	void seek( double milliseconds );

	bool loadFromFile( char* filename )
	{
		bool results = eventList.loadFromFile( filename );

		sort();

		return results;
	}

	void play()		{	isPlaying = true;	}
	void stop()		{	isPlaying = false;	}

	void update( double elapsed_milliseconds )
	{	
		if( needsSort )
		{
			sort();
		}
		seek( cur_pos_milliseconds + elapsed_milliseconds );
	}

	void sort();

	double getDuration();
	double getCurrentPosition()
	{	return cur_pos_milliseconds;	}

	void internal_addEvent( Event * e )
	{
		needsSort = true;
		events.push_back( e );
	}
	void internal_removeEvent( Event * e )
	{
		events.remove( e );
	}

// event display stuff
	DrawList * displayTarget;

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
	bool operator<( const Event &rightArg ) const
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