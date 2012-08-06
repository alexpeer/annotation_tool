#include "TheAnnotator.h"

	bool TheAnnotator::addVideoStream( char* filename )
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

	bool TheAnnotator::addAudioStream( char* filename )
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
	void TheAnnotator::updateLongestDuration( double newDuration )
	{
		if( timeline.get_endTime() < newDuration )
			timeline.set_endTime( newDuration );
		//TODO: need similar func for when removing, that checks all streams
	}

	void TheAnnotator::play()
	{			
		std::list<Stream*>::iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{
			(*it)->play();
		}

		isPlaying = true;
	}

	void TheAnnotator::stop()
	{		
		std::list<Stream*>::iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{
			(*it)->stop();
		}

		isPlaying = false;
	}

	void TheAnnotator::seek( double milliseconds )
	{
		seek_requested = true;
		seek_to = milliseconds;
	}

	void TheAnnotator::seek_percentage( double zero_to_one )
	{
		double seek_to_milliseconds = timeline.get_endTime() * zero_to_one;
		seek( seek_to_milliseconds );
	}

	void TheAnnotator::internal_seek( double milliseconds )
	{
		std::list<Stream*>::iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{	(*it)->seek( milliseconds );	}
	}
	
// update stuff
	void TheAnnotator::update()
	{

		int elapsed = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		if( seek_requested )
		{
			seek_requested = false;
			internal_seek( seek_to );
		}

		if( this->isPlaying )
		{
			//streams.
			std::list<Stream*>::iterator it;
			for( it = streams.begin(); it != streams.end(); ++it )
			{
				(*it)->update( elapsed );
			}

			timeline.update( elapsed );//_cursor( (*(streams.begin()))->getCurrentPosition() );
		}

	}

// draw stuff
	void TheAnnotator::init_draw()
	{
		canvas.add(video_area);
		canvas.add(text_area);
		canvas.add(timeline);
		canvas.add(button_play);
	}

	void TheAnnotator::resize( int width, int height )
	{
		//TODO: this should be part of some kind of layout generic

		timeline.resize( width - (width * 0.1), 150 );
		timeline.setPosition(	(width/2) - (timeline.width / 2),
								500 );

		video_area.setPosition( 0, 0 );

		button_play.setSize( 40, 40 );
		button_play.setPosition(	(width / 2) - (button_play.getBounds().width / 2), 
									40 + timeline.getBounds().height + timeline.getPosition().y 
								);
	}

	TheAnnotator::TheAnnotator()
	{
		timeline.annotator = this;
		init_draw();
		stop();

		seek_requested = false;
		seek_to = 0;

		canvas.name = "The Annotator, Canvas";
		
		button_play.theBoss = this;
	}

// cleanup stuff
	void TheAnnotator::emptyStreams()
	{
		std::list<Stream*>::iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{
			delete *it;
		}
		streams.clear();
	}

	TheAnnotator::~TheAnnotator()
	{
		emptyStreams();
	}