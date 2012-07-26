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

	BoxWithText( int x, int y, int width, int height, char * text = NULL )
	{
		sf::Sprite();

		this->setPosition( x, y );

		
		rect.sf.setSize( sf::Vector2f(width, height) );
		rect.sf.setPosition(0,0);
		words.sf.setString( text );
		words.sf.setColor( sf::Color( 255, 0, 0 ) );

		add( &rect );
		add( &words );

	}

	RectangleShape rect;
	Text words;

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

};

#endif