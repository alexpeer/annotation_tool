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

#ifndef BUTTON_PLAY_CHOOSES_NOT_TO_HATE_THE_GAME_OR_THE_PLAYER
#define BUTTON_PLAY_CHOOSES_NOT_TO_HATE_THE_GAME_OR_THE_PLAYER

#include "../generic-ui/ui.h"

class TheAnnotator; 

class Button_Play : public Button
{
public:

	//TODO: this really needs to be a toggle

	Sprite bg_over;
	Sprite bg_off;
	Sprite bg_down;
	RectangleShape box;
	TheAnnotator * theBoss;

	float width, height;

	Button_Play( )
	{
		bg_over.loadImage( "./resources/play_over.png" );
		bg_off.loadImage( "./resources/play_off.png" );
		bg_down.loadImage( "./resources/play_down.png" );

		off = &bg_off;
		down = &bg_down;
		over = &bg_over;

		add( bg_off );
		add( bg_down );
		add( bg_over );

		setSize( 40, 40 );
		//add( box );

		theBoss = NULL;

		name = "Play Button";
	}

	void setSize( float w, float h )
	{
		width = w;
		height = h;
		bg_over.setSize( width, height );
		bg_off.setSize( width, height );
		bg_down.setSize( width, height );
	}

	void onMouseClick( MouseEvent &e );

	sf::FloatRect getBounds()
	{
		return sf::FloatRect( 0, 0, width, height );
	}

	sf::FloatRect getClickBounds()
	{
		return getBounds();
	}

};

#endif