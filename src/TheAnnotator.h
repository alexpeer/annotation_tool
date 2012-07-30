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
			return false;

		
		
		streams.push_back( video );
		return true;
	}

	bool addAudioStream( char* filename )
	{
		AudioStream *audio = new AudioStream();
		bool didWork = audio->open( filename );
		if( ! didWork )
			return false;
		
		streams.push_back( audio );
		return true;
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

	}

// draw stuff
	DrawList canvas;

	DrawList video_area;
	DrawList text_area;

	Timeline timeline;

	void init_draw(int width, int height)
	{
		canvas.add(video_area);
		canvas.add(text_area);
		canvas.add(timeline);

		resize( width, height);
	}

	void resize( int width, int height )
	{
		
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