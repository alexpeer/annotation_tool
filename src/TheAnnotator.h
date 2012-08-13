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

#ifndef THEANNOTATOR_OFTEN_SEES_THE_UNEXPECTED
#define THEANNOTATOR_OFTEN_SEES_THE_UNEXPECTED

#include <SFML/Graphics.hpp>

#include "generic-ui\ui.h"

#include "Stream.h"
#include "TextStream.h"

#include "TextArea.h"
#include "Timeline.h"

#include "Video.h"

#include "ui-annotator\Buttons.h"

#include "EventStream.h"

#include <list>

class TheAnnotator
{
public:

// stream stuff
	std::list<Stream*> streams;
	bool isPlaying;

	VideoStream * addVideoStream( char* filename );
	AudioStream * addAudioStream( char* filename );
	EventStream * addEventStream( char* filename );

	//when adding streams, check to see if it is the new longest
	void updateLongestDuration( double newDuration );

	void play();

	void stop();

	void seek( double milliseconds );

	void seek_percentage( double zero_to_one );
	
	void internal_seek( double milliseconds ); // used during update

	bool seek_requested;
	double seek_to;
	
// update stuff
	sf::Clock clock;
	void update();

// draw stuff
	DrawList canvas;

	DrawList video_area;
	DrawList text_area;

	Timeline timeline;

	Button_Play button_play;

	void init_draw();

	void resize( int width, int height );

	TheAnnotator();

// cleanup stuff
	void emptyStreams();

	~TheAnnotator();
};

#endif