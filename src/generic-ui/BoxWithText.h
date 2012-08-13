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

		fixedWidth = -1;

	}

	void setText( const char* new_text )
	{
		words.sf.setString( new_text );
		
		//TODO: wrap text?

		sizeBoxToText();
	}

	void sizeBoxToText()
	//TODO: do this autmatically on text change, resize
	{		

		float padding = 8;

		sf::FloatRect bounds = words.sf.getLocalBounds();

		if( fixedWidth > 0 )	width = fixedWidth;
		else					width = bounds.width + padding;
		height = bounds.height + (bounds.height * 0.2) + padding;
			// added 20% more, as text height always seems to be underestimated

		background.sf.setSize( sf::Vector2f( width, height ) );

		words.setPosition( padding / 2, padding / 2 );

	}

	float getHeight()
	{
		return height;
	}

	RectangleShape background;
	Text words;

	float width, height;

	float fixedWidth;

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