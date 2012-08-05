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
		if( milliseconds > video.duration )
		{
			video.stop();
			return;
		}

		double result_millis = video.seek_millis( milliseconds );
		if( result_millis < milliseconds - video.millis_per_frame ||
			result_millis > milliseconds)
		// we're allowed to be back by a frame (it should handle the difference internally), but forward could be a problem
		{
			printf( "WARNING: video stream seek: %s\n\tresult not exact, off by: %f\n", video.name.c_str(), result_millis - milliseconds );
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
	AudioStream()	{	type = AUDIO;	curr_pos = 0;	}

	double curr_pos; // maintain here, to workaround "always play on seek" bug

	void play() {	audio.setPlayingOffset( sf::milliseconds( curr_pos ) );	}
	void stop()	{	audio.pause();	}
	void seek( double milliseconds )
	{
		if( milliseconds > audio.getDuration().asMilliseconds() )
		{	audio.stop();
			return;
		}

		bool wasPaused = false;

		sf::SoundSource::Status status = audio.getStatus();
		//have to be paused or playing before we can seek
		if( status == sf::SoundSource::Status::Stopped )
		{	
			//audio.play();
			audio.pause();

			wasPaused = true;
		}
		if( status == sf::SoundSource::Status::Paused )
			wasPaused = true;

		//audio.setPlayingOffset( sf::milliseconds( milliseconds ) );
		audio.pause();

		curr_pos = milliseconds;
	}

	void update( double elapsed_milliseconds )
	{	curr_pos += elapsed_milliseconds;	}

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



#endif