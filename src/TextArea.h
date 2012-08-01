#ifndef TEXTAREA_IS_WHERE_THE_READING_MAGIC_HAPPENS
#define TEXTAREA_IS_WHERE_THE_READING_MAGIC_HAPPENS

#include "generic-ui/ui.h"

class TextArea : public DrawList
{
public:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		const LinkListable *cursor = (LinkListable*)getHead();
		const Drawable *drawable = (Drawable*)cursor;

		states.transform.combine( this->getTransform() );

		float position = 0;
		float padding = 5;

		while( cursor != NULL )
		{
			drawable = (Drawable*)cursor;
			if( drawable->visible )
			{
				drawable->setPosition( sf::Vector2f( 0, position ) );
				position += drawable->
				target.draw( *drawable, states );
			}
			cursor = cursor->next;
		}
	}

};


#endif