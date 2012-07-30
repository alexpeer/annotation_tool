#ifndef STREAM_CONTAINS_THE_WISEST_FISH
#define STREAM_CONTAINS_THE_WISEST_FISH

#include "Video.h"
#include <SFML/Audio.hpp>

class Stream
{
public:

	virtual void play() = NULL;
	virtual void stop() = NULL;

	virtual void seek( double milliseconds ) = NULL;
	virtual void update( double elapsedTime ) = NULL;

	// all times should be in milliseconds
	virtual double getDuration() = NULL;
	virtual double getCurrentPosition() = NULL;

private:


};

class VideoStream : public Stream
{
public:

	void play()	{	video.play();	}
	void stop()	{	video.stop();	}
	void seek( double milliseconds )
	{
		result_millis = video.seek_millis( milliseconds );
		if( result_millis != milliseconds )
		{
			printf( "WARNING: stream seek: result not exact, off by: %d", result_millis - milliseconds );
		}
	}

	double getDuration()		{	return video.length;	}
	double getCurrentPosition	{	return video.location;	}

private:

	Video video;

};

class AudioStream : public Stream
{
public:

	void play() {	audio.play();	}
	void stop()	{	audio.pause();	}
	void seek( double milliseconds )
	{
		sf::SoundSource::Status status = audio.getStatus();
		//have to be paused or playing before we can seek
		if( status == sf::SoundSource::Status::Stopped )
		{	
			audio.play();
			audio.pause();
		}
		audio.setPlayingOffset( sf::Time( milliseconds * 1000 ) );
	}

	double getDuration()		{	return audio.length;	}
	double getCurrentPosition	{	return video.location;	}

private:
	sf::Music audio;

};

class EventStream : public Stream
{

};

#endif