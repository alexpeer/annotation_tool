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
		setFramerate( 30 ); //TODO: should pull this from file
		
		bool result = capture.read( frame );//cvQueryFrame( capture );
		if( result == false )
		{
			isLoaded = false;
			printf( "ERROR: Could not load video file: %s", filename );
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
		bool result = capture.retrieve( frame );
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
	double seek_percentage( float zero_to_one )
	{		
		//cvSetCaptureProperty( capture, CV_CAP_PROP_POS_AVI_RATIO, zero_to_one );
		capture.set( CV_CAP_PROP_POS_AVI_RATIO, zero_to_one );
		getNextFrame();
		syncLocation();
		playTimer.restart();
		return location;
	}

	double seek_millis( float millis )
	{
		//cvSetCaptureProperty( capture, CV_CAP_PROP_POS_MSEC, millis );
		capture.set( CV_CAP_PROP_POS_MSEC, millis );
		getNextFrame();
		syncLocation();
		playTimer.restart();
		return location;
	}

	void update()
	{
		if( ! isPlaying )
			return;

		sf::Time elapsed = playTimer.getElapsedTime();
		float millis_elapsed = elapsed.asMilliseconds();

		if( millis_elapsed < millis_per_frame )
			return;

		playTimer.restart();
		int result;
		while( millis_elapsed >= millis_per_frame )
		{
			//result = cvGrabFrame(capture); // should just advance frame
			result = capture.grab(); // should just advance frame
			if( result = 0 ) // retrieve error; probably end of stream?
				break;
			millis_elapsed -= millis_per_frame;
		}

		//frame = cvRetrieveFrame( capture );
		result = capture.retrieve( frame );
		syncFrameToTexture();
		syncLocation();
	}

	void play()
	{
		isPlaying = true;
		//isPaused = false;
		//isSeeking = false;

		playTimer.restart();
	}

	void stop()
	{
		isPlaying = false;
	}

};

#endif