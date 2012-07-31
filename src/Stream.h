#ifndef STREAM_CONTAINS_THE_WISEST_FISH
#define STREAM_CONTAINS_THE_WISEST_FISH

#include "Video.h"
#include <SFML/Audio.hpp>
#include <string>

class Stream
{
public:

	virtual void play() = NULL;
	virtual void stop() = NULL;

	virtual void seek( double milliseconds ) = NULL;
	virtual void update( double elapsed_milliseconds ) = NULL;

	// all times should be in milliseconds
	virtual double getDuration() = NULL;
	virtual double getCurrentPosition() = NULL;

	enum StreamType
	{
		VIDEO,
		AUDIO,
		TEXT,
		OVERLAY
	};

	StreamType type;

	std::string name;

private:

};

class VideoStream : public Stream
{
public:
	VideoStream()	{	type = VIDEO;	}

	void play()	{	video.play();	}
	void stop()	{	video.stop();	}
	void seek( double milliseconds )
	{
		double result_millis = video.seek_millis( milliseconds );
		if( result_millis != milliseconds )
		{
			printf( "WARNING: stream seek: result not exact, off by: %d", result_millis - milliseconds );
		}
	}

	void update( double elapsed_milliseconds )
	{
		video.update( elapsed_milliseconds );
	}

	double getDuration()		{	return video.duration;	}
	double getCurrentPosition()	{	return video.location;	}

	bool open( char* filename )
	{
		name = filename;
		return video.openFile( filename );
	}

	Video video;

};

class AudioStream : public Stream
{
public:
	AudioStream()	{	type = AUDIO;	}

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
		audio.setPlayingOffset(  sf::milliseconds( milliseconds ) );
	}

	void update( double elapsed_milliseconds )
	{	;	}

	double getDuration()		{	return audio.getDuration().asMilliseconds();		}
	double getCurrentPosition()	{	return audio.getPlayingOffset().asMilliseconds();	}
	
	bool open( char* filename )
	{
		name = filename;
		return audio.openFromFile( filename );
	}

private:
	sf::Music audio;

};

class TextStream : public Stream
{
public:
	TextStream()	{	type = TEXT;	}

	virtual void play()
	{
		;
	}
	virtual void stop()
	{
		;
	}

	virtual void seek( double milliseconds )
	{
		;
	}
	virtual void update( double elapsed_milliseconds )
	{
		;
	}

	// all times should be in milliseconds
	virtual double getDuration()
	{
		;
	}
	virtual double getCurrentPosition()
	{
		;
	}

};

#endif