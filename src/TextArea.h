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

#ifndef TEXTAREA_IS_WHERE_THE_READING_MAGIC_HAPPENS
#define TEXTAREA_IS_WHERE_THE_READING_MAGIC_HAPPENS

#include "generic-ui/ui.h"
#include <list>

class TextEvent_Drawable : public Drawable
{
public:

	void setLabel( const char* label_text )
	{
		label.words.sf.setCharacterSize( 10 );
		content.words.sf.setCharacterSize( 14 );

		label.background.setColor( 172, 255, 161 );
		content.background.setColor( 172, 255, 161 );

		label.words.sf.setColor( sf::Color( 0, 0, 0 ) );
		content.words.sf.setColor( sf::Color( 0, 0, 0 ) );

		content.fixedWidth = 300;

		label.setText(label_text );
		label.setPosition( sf::Vector2f( 10.0f, 0.0f ) );
		content.setPosition( sf::Vector2f( 0.0f, label.getHeight() ) );
	}

	void setContent( const char* content_text )
	{
		content.setText( content_text );
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform.combine( this->getTransform() );
		target.draw( label, states );
		target.draw( content, states );
	}

	float getHeight()
	{
		return label.getHeight() + content.getHeight();
	}

	sf::FloatRect getBounds()
	{
		//TODO: this is ugly and fragile, quickly hacked together; would just using a DrawList be fast enough, considering these are made in the draw loop?
		sf::FloatRect bounds;
		bounds = label.getBounds();
		bounds.height += content.getBounds().height;
		bounds.width = content.fixedWidth;
		return sf::FloatRect();
	}
	sf::FloatRect getClickBounds()
	{
		return sf::FloatRect();
	}

private:
	BoxWithText label; 
	BoxWithText content;
};

class TextArea : public DrawList
{
public:

	float width, height;

	//RectangleShape bg;

	TextArea()
	{
		name = "TextArea";
	}

	TextArea( float w = 300, float h = 300 )
	{
		width = w;
		height = h;
	}

	void add( Drawable * thing )
	{
		double newY = 20;
		if( getTail() != NULL)
		{
			sf::Vector2f pos = ((Drawable*)getTail())->getPosition();
			double height = ((Drawable*)getTail())->getHeight();
			newY = pos.y + height + 20;
		}

		thing->setPosition( 0, newY );
		DrawList::add( thing );
	}
	void add( Drawable & thing )
	{
		add( &thing );
	}

};


#endif