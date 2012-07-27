#ifndef VIDEO_WILL_SHOW_YOU_SOMETHING_WORTH_SEEING
#define VIDEO_WILL_SHOW_YOU_SOMETHING_WORTH_SEEING

#include "Drawable.h"
#include <SFML/Graphics.hpp>

class Video : public Sprite
{
public:
	sf::Texture texture;
	sf::Image buffer;

	Video()
	{
		buffer.create( 400, 400, sf::Color( 0, 200, 0 ) );
		texture.create( 400, 400 );
		texture.update( buffer );

		sf.setTexture(texture);
	}



};

#endif