#ifndef BOW_WITH_TEXT_IS_LIKE_A_BOX_WEARING_A_HAT_IF_THE_HAT_WERE_WORDS
#define BOW_WITH_TEXT_IS_LIKE_A_BOX_WEARING_A_HAT_IF_THE_HAT_WERE_WORDS

#include <SFML/Graphics.hpp>
#include "Drawable.h"
//#include <SFML/OpenGL.hpp>

//#include <string>

class BoxWithText :  public DrawList
{
public:
	//char * stl::String text;		

	BoxWithText( int x = 0, int y = 0, int width = 100, int height = 40, char * text = NULL )
	{
		sf::Sprite();

		this->setPosition( x, y );

		
		background.sf.setSize( sf::Vector2f(width, height) );
		background.sf.setPosition(0,0);
		words.sf.setString( text );
		words.sf.setColor( sf::Color( 255, 0, 0 ) );

		add( &background );
		add( &words );

	}

	void sizeBoxToText()
	//TODO: do this autmatically on text change, resize
	{		

		float padding = 8;

		sf::FloatRect bounds = words.sf.getLocalBounds();
		background.sf.setSize( sf::Vector2f( bounds.width + padding, bounds.height + padding ) );

		width = bounds.width + padding;
		height = bounds.height + padding;

		words.setPosition( padding / 2, padding / 2 );

	}

	RectangleShape background;
	Text words;

	float width, height;

private:
/*
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//rect.setPosition( sf::Vector2f(100,100) );

		// add this object's transform to the state
		states.transform.combine( this->getTransform() );

		// draw the components, using current state
		target.draw( rect, states );
		target.draw( words, states );

	}
*/
	
	bool isPointInside( float x, float y )
	{	return background.sf.getGlobalBounds().contains( sf::Vector2f( x, y ) );	}
};

#endif