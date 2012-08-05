#ifndef BUTTON_PLAY_CHOOSES_NOT_TO_HATE_THE_GAME_OR_THE_PLAYER
#define BUTTON_PLAY_CHOOSES_NOT_TO_HATE_THE_GAME_OR_THE_PLAYER

#include "../generic-ui/ui.h"

class TheAnnotator; 

class Button_Play : public Button
{
public:

	//TODO: this really needs to be a toggle

	Sprite bg;
	RectangleShape box;
	TheAnnotator * theBoss;

	Button_Play( )
	{
		bg.loadImage( "./resources/play.jpg" );

		off = &bg;
		down = &bg;
		over = &bg;

		add( bg );

		box.setSize( 40, 40 );
		add( box );

		theBoss = NULL;

		name = "Play Button";
	}

	void onMouseClick( MouseEvent &e );

	sf::FloatRect getBounds()
	{
		return box.sf.getLocalBounds();
	}

	sf::FloatRect getClickBounds()
	{
		return getBounds();
	}

};

#endif