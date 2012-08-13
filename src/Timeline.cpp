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

#include "Timeline.h"
#include "TheAnnotator.h"

// Tickmarks Stuff
	void Timeline_Tickmarks::onMouseClick( MouseEvent &e )
	{
		//printf( "Tickmarks: click: %f, %f\n", e.mousePos_global.x, e.mousePos_global.y );
		//printf( "\tTimeline pos: %f, %f\n", timeline->getPosition().x, timeline->getPosition().y );
		//printf( "\tTickmarks pos: %f, %f\n", getPosition().x, getPosition().y );
		
		if( e.left )
		{
			// left clicks in the tickmarks area seeks

			//get mouse location relative to tickmark area
			sf::Vector2f pos = e.getPos_local();
			float seek_to_percent = pos.x / width;

			printf( "Timeline: left click in tickmarks: %f\n", seek_to_percent );

			position_millis =  get_endTime() * seek_to_percent;
			update( 0 );

			timeline->annotator->seek_percentage( seek_to_percent );
		}
	}
	void Timeline_Tickmarks::onMouseDrag( MouseEvent &e )
	{
		if( e.left )
		{
			// left click drags in the tickmarks area seeks

			//get mouse location relative to tickmark area
			sf::Vector2f pos = e.getPos_local();
			float seek_to_percent = pos.x / width;

			printf( "Timeline: left drag in tickmarks: %f\n", seek_to_percent );

			position_millis =  get_endTime() * seek_to_percent;
			update( 0 );

			timeline->annotator->seek_percentage( seek_to_percent );
		}
	}


// Timeline Stuff
	Timeline::Timeline( )
	{
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

