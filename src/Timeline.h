#ifndef TIMELINE_WOULD_THANK_YOU_TO_AVOID_MEETING_YOURSELF_IN_THE_PAST_THANK_YOU_VERY_MUCH
#define TIMELINE_WOULD_THANK_YOU_TO_AVOID_MEETING_YOURSELF_IN_THE_PAST_THANK_YOU_VERY_MUCH

#include "generic-ui/ui.h"



// the drawable component of a stream on the timeline; the data component will reside elsewhere
class Timeline_Stream : public BoxWithText
{
public:
	Timeline_Stream(const char * name)
	{
		this->name = name;
		this->words.sf.setString( name );

		this->rect.sf.setSize( sf::Vector2f( 400, 30 ) );
	}
};

// the tickmarks area is where current / max time will be denoted; most stream nav actions will happen here
class Timeline_Tickmarks : public DrawList
{
public:
	Text startTime;
	Text endTime;


	Timeline_Tickmarks()
	{
		startTime.sf.setString("00:00:00");
		endTime.sf.setString("60:00:00");

		startTime.sf.setCharacterSize( 11 );
		endTime.sf.setCharacterSize( 11 );

		background.sf.setFillColor( sf::Color( 80, 80, 80 ) );

		add(background);
		add(startTime);
		add(endTime);

		resize( 700 );
		height = 50;
		
		
	}

	void resize( float w )
	{

		width = w;

		//startTime.setPosition( 0, height - startTime.sf.getLocalBounds().height );
		startTime.setPosition( 0, height - startTime.sf.getCharacterSize() );
		endTime.setPosition( width - endTime.sf.getLocalBounds().width, height - endTime.sf.getLocalBounds().height );

		background.sf.setSize( sf::Vector2f( width, height ) );
		
	}

	float width, height;
	
private:
	RectangleShape background;
};


// the timeline is the entire area that shows streams and stream nav; it is made of several other bits
class Timeline : public Drawable
{
public:
	Timeline()
	{
		setPosition( 50, 500 );

		resize( 700, 150 );

		background.sf.setFillColor( sf::Color( 100, 100, 100 ) );

		contents.add( background );
		contents.add( streams );
		contents.add( tickmarks );
	}

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

private:
	RectangleShape background;
	Timeline_Tickmarks tickmarks;

	DrawList contents;
	DrawList streams;

	float width, height;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform.combine( this->getTransform() );
		target.draw( contents, states );
	}
};


#endif