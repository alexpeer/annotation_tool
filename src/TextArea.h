#ifndef TEXTAREA_IS_WHERE_THE_READING_MAGIC_HAPPENS
#define TEXTAREA_IS_WHERE_THE_READING_MAGIC_HAPPENS

#include "generic-ui/ui.h"
#include <list>

class TextEvent_Drawable : public Drawable
{
public:

	void setLabel( const char* label_text )
	{
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
//		sf::FloatRect bounds;
//		bounds = label.getBounds();
//		bounds.height += content.getBounds().height;
//		bounds.width = max( label.getBounds().width + content.getBounds().width );
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

	TextArea( float w = 300, float h = 300 )
	{
		width = w;
		height = h;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		const LinkListable *cursor = (LinkListable*)getHead();
		const Drawable *drawable = (Drawable*)cursor;

		states.transform.combine( this->getTransform() );

		float position = 0;
		float padding = 5;

		TextEvent_Drawable drawing;

		std::list<TextStream*>::const_iterator it;
		for( it = streams.begin(); it != streams.end(); ++it )
		{
			// remove all DrawList children
			//this->empty();

			if( (*it)->isActive )
			{
				drawing.setLabel( (*it)->name.c_str() );
				//drawing.setContent( it->
			}
		}
		/*while( cursor != NULL )
		{
			drawable = (Drawable*)cursor;
			if( drawable->visible )
			{
				drawable->setPosition( sf::Vector2f( 0, position ) );
				position += drawable->
				target.draw( *drawable, states );
			}
			cursor = cursor->next;
		}*/
	}

	std::list<TextStream*> streams;
	void addStream( TextStream * stream )
	{
		streams.push_back( stream );
	}

};


#endif