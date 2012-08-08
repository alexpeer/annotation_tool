#include "Drawable.h";

//::::::::: LinkListable

	LinkListable::LinkListable()
	{
		owner = NULL;

		prev = NULL;
		next = NULL;
	}

	void LinkListable::remove()
	{
		if(this->owner == NULL)
			return;

		next->prev = prev;
		prev->next = next;
		//notify Reaper of orphan? //TODO: invent Reaper

		if( owner->head == this )
			owner->head = NULL;
		if( owner->tail == this )
			owner->tail = NULL;
		owner->size--;

		this->owner = NULL;
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
		thing->remove();

		if(head == NULL)
		{	head = thing;
			tail = thing;
		}
		else{
			tail->next = thing;
			thing->prev = tail;
			tail = thing;
		}

		thing->owner = this;
		size++;
	}

	void LinkList::remove( LinkListable *thing )
	{
		if( head == thing )		{	head = thing->next;		}
		if( tail == thing )		{	tail = thing->prev;		}

		thing->remove();

		size--;
	}

	void LinkList::empty( )
	{
		LinkListable *cursor = head;
		LinkListable *farewell;

		while( cursor != NULL )
		{
			farewell = cursor;
			cursor = cursor->next;
			farewell->remove();
		}

		head = NULL;
		tail = NULL;
		size = 0;
	}

	LinkListable* LinkList::getHead() const		{	return head;	}

//:::::::: Drawable
		
	sf::Vector2f Drawable::getParentPosition()
	{

		DrawList* parent = (DrawList*) owner;

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

		//printf( "%s, Mousing, up\n", name.c_str() );
		
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