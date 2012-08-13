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

#include "Drawable.h";

//::::::::: LinkListable

	LinkListable::LinkListable()
	{
		parent = NULL;

		prev = NULL;
		next = NULL;
	}

	void LinkListable::removeFromParent()
	{
		if(this->parent == NULL)
			return;

		if( next )	next->prev = prev;
		if( prev )	prev->next = next;
		//notify Reaper of orphan? //TODO: invent Reaper

		if( parent->head == this )		parent->head = next;
		if( parent->tail == this )		parent->tail = prev;
		parent->size--;

		this->parent = NULL;
	}

/*	void LinkListable::append( LinkListable *thing )
	{	
		thing->remove();
		
		thing->prev = this;
		thing->next = this->next;
		this->next = thing;

		if( owner->head == this )
			owner->head == thing;
		owner->size++;

		thing->owner = owner;
	}

	void LinkListable::prepend( LinkListable *thing )
	{
		thing->remove();

		thing->next = this;
		thing->prev = this->prev;
		this->prev = thing;

		if( owner->tail == this )
			owner->tail == thing;
		owner->size++;

		thing->owner = owner;
	}
*/

//:::::::::: LinkList

	LinkList::LinkList()
	{
		head = NULL;
		tail = NULL;
		size = 0;
	}

	void LinkList::add( LinkListable *thing )
	{		
		thing->removeFromParent();

		if(head == NULL)
		{	head = thing;
			tail = thing;
		}
		else{
			tail->next = thing;
			thing->prev = tail;
			tail = thing;
		}

		thing->parent = this;
		size++;
	}

	void LinkList::remove( LinkListable *thing )
	{
		if( this != thing->parent )
			return;

		thing->removeFromParent();
		/*
		if( head == thing )		{	head = thing->next;		}
		if( tail == thing )		{	tail = thing->prev;		}

		size--;
		*/
	}

	void LinkList::empty( )
	{
		LinkListable *cursor = head;
		LinkListable *farewell;

		while( cursor != NULL )
		{
			farewell = cursor;
			cursor = cursor->next;
			farewell->removeFromParent();
		}

		head = NULL;
		tail = NULL;
		size = 0;
	}

	LinkListable* LinkList::getHead() const		{	return head;	}
	
	LinkListable* LinkList::getTail() const		{	return tail;	}

//:::::::: Drawable
		
	sf::Vector2f Drawable::getParentPosition()
	{

		DrawList* parent = (DrawList*) parent;

		sf::Vector2f parentPos;
		parentPos.x = 0;
		parentPos.y = 0;
		if(parent != NULL)
			parentPos = parent->getParentPosition();

		//printf( "GETPARENTPOS: %s\n", this->name.c_str() ); //DEBUG//
		sf::Vector2f thisPos = this->getPosition();
		parentPos += thisPos;

		return parentPos;
	}

//:::::::: DrawList		

	void DrawList::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{

		const LinkListable *cursor = (LinkListable*)getHead();
		const Drawable *drawable = (Drawable*)cursor;

		states.transform.combine( this->getTransform() );

		while( cursor != NULL )
		{
			drawable = (Drawable*)cursor;
			if( drawable->visible )
				target.draw( *drawable, states );

			cursor = cursor->next;
		}

	}

	float DrawList::getHeight( )
	{
		LinkListable *cursor = (LinkListable*)getHead();
		Drawable *drawable = (Drawable*)cursor;

		float biggest = 0;
		float thisHeight = 0;

		while( cursor != NULL )
		{
			drawable = (Drawable*)cursor;

			thisHeight = drawable->getHeight();
			if(thisHeight > biggest )
				biggest = thisHeight;

			cursor = cursor->next;
		}

		return biggest;

	}

	bool DrawList::isPointInside( float x, float y )
	{
		for( const LinkListable *cursor = (LinkListable*)getHead(); 
			 cursor != NULL;
			 cursor = cursor->next
		   )
		{
			if( ((Drawable*)cursor)->isPointInside( x, y ) )
				return true;
		}
		return false;
	}

	sf::FloatRect DrawList::getBounds()
	{
		float left, right, top, bottom;	
		LinkListable *cursor = (LinkListable*)getHead();
		Drawable *drawable = (Drawable*)cursor;

		if( cursor == NULL ) //empty list
			return sf::FloatRect( 0, 0, 0, 0 );

		sf::FloatRect cb; //cursorBounds
		cb = drawable->getBounds();
		cursor = cursor->next;

		left = cb.left;
		top = cb.top;

		right = cb.left + cb.width;
		bottom = cb.top + cb.height;

		while( cursor != NULL )
		{
			drawable = (Drawable*)cursor;

			cb = drawable->getBounds();
			if( left > cb.left )	left = cb.left;
			if( top > cb.top )		top = cb.top;
			if( right < cb.width + cb.left )	right = cb.width + cb.left;
			if( bottom < cb.height + cb.top )	bottom = cb.height + cb.top;

			cursor = cursor->next;
		}

		return sf::FloatRect( left, top, right - left, bottom - top );
	}
	sf::FloatRect  DrawList::getClickBounds()
	{
		float left, right, top, bottom;	
		LinkListable *cursor = (LinkListable*)getHead();
		Drawable *drawable = (Drawable*)cursor;

		//empty list, return empty bounds
		if(cursor == NULL)
			return sf::FloatRect( 0, 0, 0, 0 );

		sf::FloatRect cb; //cursorBounds
		cb = drawable->getClickBounds();
		cursor = cursor->next;

		left = cb.left;
		top = cb.top;

		right = cb.left + cb.width;
		bottom = cb.top + cb.height;

		while( cursor != NULL )
		{
			drawable = (Drawable*)cursor;

			cb = drawable->getClickBounds();
			if( left > cb.left )				left = cb.left;
			if( top > cb.top )					top = cb.top;
			if( right < cb.width + cb.left )	right = cb.width + cb.left;
			if( bottom < cb.height + cb.top )	bottom = cb.height + cb.top;

			cursor = cursor->next;
		}

		return sf::FloatRect( left, top, right - left, bottom - top );
	}

	// DrawList mouseable stuff, for passing internal mouse event handling down the tree
	
	void DrawList::internal_onMouseMove( MouseEvent e )
	{
		//printf( "%s, Mousing, move\n", name.c_str() );
		
		this->internal_mousable.internal_onMouseMove( e );

		// have to do this again here, as we aren't passing the value back out from internal_mousable; using this elsewhere for de-facto "pop" of state stack, reverting transform.  So, this is a neccesary hack for the moment.
		e.transform.combine( getTransform() );

		if( areChildrenMousable == false )
			return;

		for( const LinkListable *cursor = (LinkListable*)getHead(); 
			 cursor != NULL;
			 cursor = cursor->next
		   )
		{
			((Drawable*)cursor)->internal_onMouseMove( e );				
		}
	}
	void DrawList::internal_onMouseDown( MouseEvent e )
	{

		//printf( "%s, Mousing, down\n", name.c_str() );
		
		this->internal_mousable.internal_onMouseDown( e );

		// have to do this again here, as we aren't passing the value back out from internal_mousable; using this elsewhere for de-facto "pop" of state stack, reverting transform.  So, this is a neccesary hack for the moment.
		e.transform.combine( getTransform() );

		if( areChildrenMousable == false )
			return;

		for( const LinkListable *cursor = (LinkListable*)getHead(); 
			 cursor != NULL;
			 cursor = cursor->next
		   )
		{
			((Drawable*)cursor)->internal_onMouseDown( e );				
		}
	}

	void DrawList::internal_onMouseUp( MouseEvent e )
	{

		printf( "%s, Mousing, up\n", name.c_str() );
		
		this->internal_mousable.internal_onMouseUp( e );

		// have to do this again here, as we aren't passing the value back out from internal_mousable; using this elsewhere for de-facto "pop" of state stack, reverting transform.  So, this is a neccesary hack for the moment.
		e.transform.combine( getTransform() );

		if( areChildrenMousable == false )
			return;

		for( const LinkListable *cursor = (LinkListable*)getHead(); 
			 cursor != NULL;
			 cursor = cursor->next
		   )
		{
			((Drawable*)cursor)->internal_onMouseUp( e );				
		}
	}