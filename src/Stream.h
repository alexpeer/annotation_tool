//    Copyright Alex Peer 2012.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STREAM_CONTAINS_THE_WISEST_FISH
#define STREAM_CONTAINS_THE_WISEST_FISH

#include "Video.h"
#include <SFML/Audio.hpp>
#include <string>

class Stream
{
public:

	Stream()
	{
		offset = 0;
		isPlaying = false;
	}

	virtual void play() = NULL;
	virtual void stop() = NULL;

	virtual void seek( double milliseconds ) = NULL;
	virtual void update( double elapsed_milliseconds ) = NULL;

	// all times should be in milliseconds
	virtual double getDuration() = NULL;			// TODO: change to getEndTime
	virtual double getCurrentPosition() = NULL;

	virtual void setOffset( double new_offset )
	{
		offset = new_offset;
	}

	double offset;
	bool isPlaying;

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

	void play()	{	video.play();	isPlaying = true;	}
	void stop()	{	video.stop();	isPlaying = false;	}
	void seek( double milliseconds )
	{
		double seek_to_adj = milliseconds - offset;

		if( seek_to_adj > video.duration )
		{
			video.stop();
			return;
		}

		if( seek_to_adj < 0 )
		{
			video.stop();
			return;
		}

		double result_millis = video.seek_millis( seek_to_adj );
		if( result_millis < seek_to_adj - video.millis_per_frame ||
			result_millis > seek_to_adj)
		// we're allowed to be back by a frame (it should handle the difference internally), but forward could be a problem
		{
			printf( "WARNING: video stream seek: %s\n\tresult not exact, off by: %f\n", video.name.c_str(), result_millis - seek_to_adj );
		}

		if( isPlaying )			video.play();
		else					video.stop();
	}

	void update( double elapsed_milliseconds )
	{
		video.update( elapsed_milliseconds );
	}

	double getDuration()		{	return offset + video.duration;	}
	double getCurrentPosition()	{	return offset + video.location;	}

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

	void play() 
	{	
		isPlaying = true;

		double seek_to_adj = curr_pos - offset;
		if( seek_to_adj > 0 )
			audio.setPlayingOffset( sf::milliseconds( seek_to_adj ) ); 
		else
			audio.stop();
	}
	void stop()	{	audio.pause(); isPlaying = false;	}
	void seek( double milliseconds )
	{
		double seek_to_adj = milliseconds - offset;

		if( seek_to_adj > audio.getDuration().asMilliseconds() )
		{	
			curr_pos = milliseconds;
			audio.stop();
			return;
		}

		if( seek_to_adj < 0 )
		{
			curr_pos = milliseconds;
			audio.stop();
			return;
		}

		//bool wasPaused = false;
		sf::SoundSource::Status status = audio.getStatus();
		//have to be paused or playing before we can seek
		if( status == sf::SoundSource::Status::Stopped )
		{	
		//	//audio.play();
			audio.pause();
		//
		//	wasPaused = true;
		}
		//if( status == sf::SoundSource::Status::Paused )
		//	wasPaused = true;

		//audio.setPlayingOffset( sf::milliseconds( seek_to_adj ) );
		//audio.pause();

		if( isPlaying )			audio.setPlayingOffset( sf::milliseconds( seek_to_adj ) );//audio.play();
		//else					audio.pause();

		curr_pos = milliseconds;
	}

	void update( double elapsed_milliseconds )
	{	
		if( curr_pos - offset < 0 && curr_pos - offset + elapsed_milliseconds > 0 )
			seek( curr_pos + elapsed_milliseconds );
		curr_pos += elapsed_milliseconds;
	}

	void setOffset( double new_offset )
	{
		offset = new_offset;
		seek( curr_pos );
	}

	double getDuration()		{	return offset + audio.getDuration().asMilliseconds();		}
	double getCurrentPosition()	{	return offset + audio.getPlayingOffset().asMilliseconds();	}
	
	bool open( char* filename )
	{
		name = filename;
		return audio.openFromFile( filename );
	}

private:
	sf::Music audio;

};



#endif