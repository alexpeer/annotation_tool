#ifndef THEANNOTATOR_OFTEN_SEES_THE_UNEXPECTED
#define THEANNOTATOR_OFTEN_SEES_THE_UNEXPECTED

#include <SFML/Graphics.hpp>

#include "generic-ui\ui.h"
#include "Stream.h"
#include "Timeline.h"
#include "Video.h"

#include <list>

class TheAnnotator
{
public:

// stream stuff
	std::list<Stream*> streams;

	bool addVideoStream( char* filename )
	{
		VideoStream *video = new VideoStream();
		bool didWork = video->open( filename );
		if( ! didWork )
		{
			printf( "ERROR: Couldn't load video file: %s\n", filename );
			return false;
		}
		
		video_area.add( video->video );

		streams.push_back( video );
		updateLongestDuration( video->getDuration() );
		return true;
	}

	bool addAudioStream( char* filename )
	{
		AudioStream *audio = new AudioStream();
		bool didWork = audio->open( filename );
		if( ! didWork )
		{
			printf( "ERROR: couldn't load audio file: %s\n", filename );
			return false;
		}
		
		streams.push_back( audio );
		updateLongestDuration( audio->getDuration() );
		return true;
	}

	//when adding streams, check to see if it is the new longest
	void updateLongestDuration( double newDuration )
	{
		if( timeline.get_endTime() < newDuration )
			timeline.set_endTime( newDuration );
		//TODO: need similar func for when removing, that checks all streams
	}

	void play()
	{			
		std::list<Stream*>::iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{
			(*it)->play();
		}
	}

	void stop()
	{		
		std::list<Stream*>::iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{
			(*it)->stop();
		}
	}

	void seek( double milliseconds )
	{
		std::list<Stream*>::iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{	(*it)->seek( milliseconds );	}

	}

	void seek_percentage( double zero_to_one )
	{
		double seek_to_milliseconds = timeline.get_endTime() * zero_to_one;
		seek( seek_to_milliseconds );
	}
	
// update stuff
	sf::Clock clock;
	void update()
	{

		int elapsed = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		//streams.
		std::list<Stream*>::iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{
			(*it)->update( elapsed );
		}

		timeline.update( elapsed );//_cursor( (*(streams.begin()))->getCurrentPosition() );
	}

// draw stuff
	DrawList canvas;

	DrawList video_area;
	DrawList text_area;

	Timeline timeline;

	void init_draw()
	{
		canvas.add(video_area);
		canvas.add(text_area);
		canvas.add(timeline);
	}

	void resize( int width, int height )
	{
		;
	}

	TheAnnotator()
	{
		timeline.annotator = this;
		init_draw();
	}

// cleanup stuff
	void emptyStreams()
	{
		std::list<Stream*>::iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{
			delete *it;
		}
		streams.clear();
	}

	~TheAnnotator()
	{
		emptyStreams();
	}
};

#endif