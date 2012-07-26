#ifndef TIMELINE_WOULD_THANK_YOU_TO_AVOID_MEETING_YOURSELF_IN_THE_PAST_THANK_YOU_VERY_MUCH
#define TIMELINE_WOULD_THANK_YOU_TO_AVOID_MEETING_YOURSELF_IN_THE_PAST_THANK_YOU_VERY_MUCH

#include "Drawable.h"
#include "BoxWithText.h"



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

// th tickmarks area is where current / max time will be denoted; most stream nav actions will happen here
class Timeline_Tickmarks : public DrawList
{

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