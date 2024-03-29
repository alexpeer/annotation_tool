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

#include "Mousable.h"

//MouseStuff
	void MouseStuff::internal_onMouseMove( MouseEvent e )
	{
		//add chief's transform to event's working copy
		e.transform.combine( chief->getTransform() );
		e.transform_inverse = e.transform.getInverse();

		sf::Vector2f mousePos_local = e.getPos_local();
		
		e.left = leftDown;
		e.right = rightDown;
		e.middle = middleDown;

		//DEBUG//
		//printf("\tMousePos_G: %d, %d\n", (int)e.mousePos_global.x, (int)e.mousePos_global.y );
		//printf("\tMousePos_L: %d, %d\n", (int)mousePos_local.x, (int)mousePos_local.y);
		//DEBUG//

		// check for drag start
		//   if mouse was down inside the object and we're moving, that's a drag
		if( wasDownIn )
		{
			wasDragging = true;
		}

		sf::FloatRect bounds = chief->getClickBounds();
		if( bounds.contains( mousePos_local ) )
		{
		// if mouse is inside object, do mouseover
			if( wasMouseOver == false )
			{
				wasMouseOver = true;
				chief->onMouseOver( e );
			}
		}
		else
		{
		// otherwise, we're not inside the object
			if( wasMouseOver == true )
			{
				// if we were, and now aren't, handle mouse out event
				wasMouseOver = false;
				chief->onMouseOut( e );
			}
		}

		if( wasDragging )
		{
			e.mouseDownPoint_local = this->mouseDownPoint_local;
			chief->onMouseDrag( e );
		}
	}

	void MouseStuff::internal_onMouseDown( MouseEvent e )
	{		
		//add chief's transform to event's working copy
		e.transform.combine( chief->getTransform() );
		e.transform_inverse = e.transform.getInverse();

		sf::Vector2f mousePos_local = e.getPos_local();

		if( e.button == sf::Mouse::Button::Left )			leftDown = true;
		else if( e.button == sf::Mouse::Button::Right )		rightDown = true;
		else if( e.button == sf::Mouse::Button::Middle )	middleDown = true;

		sf::FloatRect bounds = chief->getClickBounds();
		if( bounds.contains( mousePos_local ) )
		{
		// mouse is clicked and inside shape
			mouseDownPoint_local = mousePos_local; //need to know anchor for drag
			wasDownIn = true;
		}
	}

	void MouseStuff::internal_onMouseUp( MouseEvent e )
	{
		//add chief's transform to event's working copy
		e.transform.combine( chief->getTransform() );
		e.transform_inverse = e.transform.getInverse();

		sf::Vector2f mousePos_local = e.getPos_local();

		if( e.button == sf::Mouse::Button::Left )			leftDown = false;
		else if( e.button == sf::Mouse::Button::Right )		rightDown = false;
		else if( e.button == sf::Mouse::Button::Middle )	middleDown = false;

		e.left = leftDown;
		e.right = rightDown;
		e.middle = middleDown;

		sf::FloatRect bounds = chief->getClickBounds();
		if( bounds.contains( mousePos_local ) )
		{
			// button down 
			e.mouseDownPoint_local = mouseDownPoint_local;
			chief->onMouseClick( e );
		}
		
		//check for drag
		if( wasDownIn == true && wasDragging == true )
		{
			e.mouseDownPoint_local = mouseDownPoint_local;
			chief->onMouseDragRelease( e );
		}

		if( !leftDown && !rightDown && !middleDown )
		{
			wasDownIn = false;
			wasDragging = false;
		}
	}