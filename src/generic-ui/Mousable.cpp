#include "Mousable.h"

//MouseStuff
	void MouseStuff::internal_onMouseMove( MouseEvent e )
	{
		//add chief's transform to event's working copy
		e.transform.combine( chief->getTransform() );
		e.transform_inverse = e.transform.getInverse();

		sf::Vector2f mousePos_local = e.getPos_local();

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

		wasDownIn = false;
		wasDragging = false;
	}