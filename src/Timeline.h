#ifndef TIMELINE_WOULD_THANK_YOU_TO_AVOID_MEETING_YOURSELF_IN_THE_PAST_THANK_YOU_VERY_MUCH
#define TIMELINE_WOULD_THANK_YOU_TO_AVOID_MEETING_YOURSELF_IN_THE_PAST_THANK_YOU_VERY_MUCH

#include "generic-ui/ui.h"

class TheAnnotator;


// the drawable component of a stream on the timeline; the data component will reside elsewhere
class Timeline_Stream : public BoxWithText
{
public:
	Timeline_Stream(const char * name)
	{
		this->name = name;
		this->words.sf.setString( name );

		this->background.sf.setSize( sf::Vector2f( 400, 30 ) );
	}
};

// the tickmarks area is where current / max time will be denoted; most stream nav actions will happen here
class Timeline_Tickmarks : public DrawList
{
public:

	Timeline_Tickmarks()
	{
		areChildrenMousable = false;

		startTime.sf.setString("00:00:00");
		endTime.sf.setString("60:00:00");

		startTime.sf.setCharacterSize( 11 );
		endTime.sf.setCharacterSize( 11 );

		background.sf.setFillColor( sf::Color( 80, 80, 80 ) );

		cursor.sf.setFillColor( sf::Color( 190, 232, 224 ) );
		cursorTime.words.sf.setCharacterSize( 11 );
		cursorTime.words.sf.setColor( sf::Color( 0, 0, 0 ) );
		cursorTime.background.sf.setFillColor( sf::Color( 190, 232, 224 ) );


		add(background);
		add(cursor);
		add(cursorTime);
		add(startTime);
		add(endTime);

		height = 50;
		resize( 700 );
		
		position_millis = 0;
		position_percent = 0;
		update( 0 );

		background.name = "Tickmarks background";
		name = "Timeline tickmarks";
	}

	void resize( float w )
	{

		width = w;

		//startTime.setPosition( 0, height - startTime.sf.getLocalBounds().height );
		startTime.setPosition( 0, height - startTime.sf.getCharacterSize() );
		endTime.setPosition( width - endTime.sf.getLocalBounds().width, height - endTime.sf.getLocalBounds().height );

		background.sf.setSize( sf::Vector2f( width, height ) );
		
		cursor.sf.setSize( sf::Vector2f( 1, height+2 ) );

	}


	void update( double elapsed )
	{
		position_millis += elapsed;
		position_percent = position_millis / endTime_millis;

		//move the cursor to indicate current playback position
		float move_to_x = width * position_percent;

		cursor.sf.setPosition( move_to_x, 0 );

		setWordsToMillis( position_millis, cursorTime.words );

		//center the text under the cursor
		cursorTime.sizeBoxToText();
		cursorTime.setPosition(		(int) (move_to_x - (cursorTime.width / 2)),
									(int)(cursor.getPosition().y + cursor.sf.getLocalBounds().height)
							  );
	}

	void set_endTime( double milliseconds )
	{
		endTime_millis = milliseconds;
		setWordsToMillis( milliseconds, endTime );
	}

	double get_endTime( )
	{
		return endTime_millis;
	}

	void setWordsToMillis( const double &millis, Text &words )
	{
		char buffer[9];

		int seconds = millis / 1000;
		int minutes = seconds / 60;
		seconds = seconds % 60;
		int hours = minutes / 60;
		minutes = minutes % 60;
		
		sprintf( buffer, "%02d:%02d:%02d", hours, minutes, seconds );
		
		words.sf.setString( buffer );

	}

	float width, height;

//private:
	RectangleShape background;

	Text startTime;
	Text endTime;

	double endTime_millis;

	double position_millis;
	double position_percent;

	RectangleShape cursor;
	BoxWithText cursorTime;
};

// the timeline is the entire area that shows streams and stream nav; it is made of several other bits
class Timeline : public DrawList
{
public:
friend TheAnnotator;

	Timeline();

	void resize( float w, float h)
	{
		width = w;
		height = h;
		
		background.sf.setSize( sf::Vector2f( width, height ) );

		tickmarks.height = 50;
		tickmarks.setPosition( 0, height - tickmarks.height );
		tickmarks.resize( w );

		//TODO: resize all components
	}

	void update( double elapsed )
	{		
		tickmarks.update( elapsed );
	}
	
	void set_endTime( double milliseconds )
	{
		tickmarks.set_endTime( milliseconds );
	}

	double get_endTime()
	{
		return tickmarks.get_endTime();
	}

private:
	RectangleShape background;
	Timeline_Tickmarks tickmarks;

	DrawList contents;
	DrawList streams;

	float width, height;

	TheAnnotator * annotator;

	//void draw(sf::RenderTarget& target, sf::RenderStates states) const
	//{
	//	states.transform.combine( this->getTransform() );
	//	target.draw( contents, states );
	//}

	bool isPointInside( float x, float y )
	{
		return background.isPointInside( x, y );
	}

	bool onMouseMove( float x, float y )
	{
		return true;
	}

	bool onMouseClick( float x, float y, sf::Mouse::Button which );

	bool onMouseDrag( float x, float y, sf::Mouse::Button which )
	{
		return true;
	}
};


#endif