#include "Timeline.h"
#include "TheAnnotator.h"

	Timeline::Timeline( )
	{
		areChildrenMousable = false;

		setPosition( 50, 500 );

		resize( 700, 150 );

		background.sf.setFillColor( sf::Color( 100, 100, 100 ) );

		add(contents);
		contents.add( background );
		contents.add( streams );
		contents.add( tickmarks );

		background.name = "Timeline bg";
		streams.name = "Timeline streams";
		tickmarks.name = "Timeline tickmarks";
		contents.name = "Timeline contents";
	}

	bool Timeline::onMouseClick( float x, float y, sf::Mouse::Button which )
	{
		printf( "Timeline: click: %f, %f\n", x, y );
		printf( "\tTimeline pos: %f, %f\n", this->getPosition().x, this->getPosition().y );
		printf( "\tTickmarks pos: %f, %f\n", tickmarks.getPosition().x, tickmarks.getPosition().y );



		if( which == sf::Mouse::Left )
		{
			// left clicks in the tickmarks area seeks
			if( tickmarks.isPointInside( x, y ) )
			{

				//get mouse location relative to tickmark area
				sf::Vector2f parentPos = tickmarks.getParentPosition();
				sf::Vector2f pos = tickmarks.getPosition();
				pos += parentPos;
				float seek_to_percent = (x - pos.x) / tickmarks.width;

				printf( "Timeline: left click in tickmarks: %f\n", seek_to_percent );

				tickmarks.position_millis =  get_endTime() * seek_to_percent;
				update( 0 );

				annotator->seek_percentage( seek_to_percent );

			}
		}

		return true;
	}