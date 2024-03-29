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

#ifndef VIDEO_WILL_SHOW_YOU_SOMETHING_WORTH_SEEING
#define VIDEO_WILL_SHOW_YOU_SOMETHING_WORTH_SEEING

#include "generic-ui/ui.h"
#include <SFML/Graphics.hpp>
//#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

class Video : public Sprite
{
//uses openCV to read frames from a file, and putsit on an SFML texture to display via OpenGL
// TODO: use a proper lib to open the video -- gstreamer or maybe some version of libavcodec / ffmpeg that has patches for accurate seeking ( do they all have that now? )
public:
	// SFML stuff
	sf::Texture texture;
	sf::Image buffer;
	sf::Clock playTimer;

	// openCV stuff

	//CvCapture* capture;
	cv::VideoCapture capture;
    //IplImage* frame;
	cv::Mat frame;

	// general player stuff
	bool isPlaying;
	//bool isSeeking = false;
	//bool isPaused = false;
	double location;

	double duration;

	bool isLoaded;

	float framerate_per_second;
	float millis_per_frame;

	float width, height;

	Video()
	{
		isPlaying = false;
		location = 0;

		isLoaded = false;
		capture = NULL;

		framerate_per_second = 30;
		millis_per_frame = 30000;


		//buffer.create( 400, 400, sf::Color( 0, 200, 0 ) );
		//texture.create( 400, 400 );
		//texture.update( buffer );

		//sf.setTexture(texture);
	}

	~Video()
	{
	//	if( capture != NULL )
	//		cvReleaseCapture( &capture );
	}

	bool openFile( char * filename )
	{
		//capture = cvCreateFileCapture( filename );
		capture.open( filename );
		//TODO: check for errors on file open

		int fps = capture.get( CV_CAP_PROP_FPS );
		setFramerate( fps ); //TODO: should pull this from file

		duration = capture.get( CV_CAP_PROP_FRAME_COUNT ) * fps;
		
		bool result = capture.read( frame );//cvQueryFrame( capture );
		if( result == false )
		{
			isLoaded = false;
			printf( "ERROR: Could not load video file: %s\n", filename );
			return false;
		}

		isLoaded = true;

		width = frame.cols;
		height = frame.rows;

		buffer.create( width, height, sf::Color( 0, 200, 0 ) );
		texture.create( width, height );

		sf.setTexture(texture);
		
		syncFrameToTexture();
		syncLocation();

		return true;
	}

	void syncLocation()
	{
		location = capture.get( CV_CAP_PROP_POS_MSEC );//cvGetCaptureProperty( capture, CV_CAP_PROP_POS_MSEC );
	}

	void setFramerate(float frames_per_second)
	{
		framerate_per_second = frames_per_second;
		millis_per_frame =  1000 / framerate_per_second;
	}

	void advanceFrame()
	{
        bool result = capture.grab();//cvGrabFrame(capture);
	}

//	void getFrame()
//	{
//		frame = cvRetrieveFrame( capture );
//	}

	void getNextFrame()
	{
		//frame = cvQueryFrame( capture );
		bool result = capture.grab( );
		result = capture.retrieve( frame );
	}

	void syncFrameToTexture()
	// copy the image retrived from the file to an openGL texture we can draw
	{
		//if(frame == NULL)
		//	return;

		uchar * start = frame.data;
		//int imageSize_bytes = frame.step * frame.rows;
		int imageWidth = frame.cols;
		int imageHeight = frame.rows;

		//DEBUG//
		if( frame.rows == 0 )
			printf( "ERROR: Image frames went to 0.\n" );
		//DEBUG//

		char red, green, blue;
		uchar * src;

		for( int i = 0; i < imageHeight; i++ )
		{
			src = start + (frame.step * i);

			for( int j = 0; j < imageWidth; j++ )
			{
				//TODO: this should only read IPL_DEPTH_8U
				blue = *src;
				src++;
				green = *src;
				src++;
				red = *src;

				buffer.setPixel(j, i, sf::Color(red, green, blue) );

				src++;
			}

		}

		//buffer.loadFromMemory( src, frame->imageSize );
		texture.update( buffer );
	}

	// seek types in openCV:
	//CV_CAP_PROP_POS_MSEC
	//CV_CAP_PROP_POS_FRAMES
	//CV_CAP_PROP_POS_AVI_RATIO
	//TODO for all seek: double check seek is accurate 
/*	//TODO: let's always use millis, so all streams speak in the same units
	double seek_percentage( float zero_to_one )
	{		
		//cvSetCaptureProperty( capture, CV_CAP_PROP_POS_AVI_RATIO, zero_to_one );
		capture.set( CV_CAP_PROP_POS_AVI_RATIO, zero_to_one );
		getNextFrame();
		syncLocation();
		playTimer.restart();
		millis_since_last_frame = 0;
		return location;
	}
*/
	double seek_millis( float millis )
	{
		//cvSetCaptureProperty( capture, CV_CAP_PROP_POS_MSEC, millis );
		capture.set( CV_CAP_PROP_POS_MSEC, millis );
		getNextFrame();
		syncLocation();
		if( ! isPlaying )	syncFrameToTexture(); // display frame now, if not playing
		playTimer.restart();
		millis_since_last_frame = millis - location;
		return location;
	}

	void update( double elapsed_milliseconds )
	{
		if( ! isPlaying )
			return;

		//sf::Time elapsed_milliseconds = playTimer.getElapsedTime().asMilliseconds();

		millis_since_last_frame += elapsed_milliseconds;

		total_elapsed_playtime += elapsed_milliseconds;
		//printf( "Video: %s\n\tTotal Playime: %f", name.c_str(), total_elapsed_playtime );

		if( millis_since_last_frame < millis_per_frame )
			return;
		//playTimer.restart();

		float curr_pos = location;

		int result;
		while( millis_since_last_frame >= millis_per_frame )
		{
			result = capture.grab(); // should just advance frame
			if( result == false ) // retrieve error; probably end of stream?
			{	
				stop();
				break;
			}
			millis_since_last_frame -= millis_per_frame;
		}

		if( isPlaying )
		{
			//frame = cvRetrieveFrame( capture );
			result = capture.retrieve( frame );
			syncFrameToTexture();
			syncLocation();

			//DEBUG//
		
			//	printf( "\ncurr_pos: %f\n", curr_pos );
			//	printf( "location: %f\n", location );
			//	printf( "elapsed : %f\n", total_elapsed_playtime );
			//	printf( "diff    : %f\n", curr_pos - location );
			//	printf( "dur: %f\n", duration );

			//DEBUG//
		}
	}

	float millis_since_last_frame;
	float total_elapsed_playtime;

	void play()
	{
		isPlaying = true;
		//isPaused = false;
		//isSeeking = false;

		playTimer.restart();
		millis_since_last_frame = 0;
		total_elapsed_playtime = 0;
	}

	void stop()
	{
		isPlaying = false;
	}

};

#endif