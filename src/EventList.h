#ifndef EVENTLIST_MAYBE_DID_START_THE_FIRE
#define EVENTLIST_MAYBE_DID_START_THE_FIRE

// events are things that happen; they are meant to be added to a timeline and be executed on play and on seek

class Event
{
public:

	virtual void addTo( EventStream &stream ) = 0;
	virtual void removeFrom( EventStream &stream ) = 0;

	virtual void activate() = 0;
	virtual void deactivate() = 0;
	// activate and deactivate are expected to be inverse actions

	bool isActiveAtTime( double milliseconds ) = 0;	
	
	bool operator<( Event &rightArg )
	{
		return (this->when < rightArg.when);
	}

	Event * cancelsThis;
	// events can cancel others; used to for faster seek

	double when; // in milliseconds
}

class EventInverse : public Event
{
public:

	Event * inverseOf;

	EventInverse( Event * inverseOf )
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

}

// events can be instantaneous, or can have start and end durations; if they have durations, they'll have start and stop actions
class EventWithDuration : public Event
{
public:

	virtual void addTo( EventStrea &stream )
	{
		//assume duration events cancel each other
		startEvent->cancelsThis = stopEvent;
		stopEvent->cancelsThis = startEvent;

		stream.add( startEvent );
		stream.add( stopEvent );
	}

	virtual void removeFrom( EventStream &stream )
	{
		stream.remove( startEvent );
		stream.remove( stopEvent );
	}

	bool isActiveAtTime( double milliseconds )
	{
		if( milliseconds >= startEvent->when && milliseconds <= stop->when )
			return true;
		return false;
	}

	Event *startEvent;
	Event *stopEvent;
}


// event list holds Events, which describe things that happen
//TODO: maybe this is just an std::list?

#endif