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
		//notify Reaper of orphan?

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

	bool DrawList::onMouseMove( float x, float y )
	{		
		if( areChildrenMousable == false )
			return false;

		bool result = false;
		for( const LinkListable *cursor = (LinkListable*)getHead(); 
			 cursor != NULL;
			 cursor = cursor->next
		   )
		{
			if( ((Drawable*)cursor)->onMouseMove( x, y ) )
				result = true;
		}
		return result;
	}
	bool DrawList::onMouseClick( float x, float y, sf::Mouse::Button which )
	{
		if( areChildrenMousable == false )
			return false;

		bool result = false;
		for( const LinkListable *cursor = (LinkListable*)getHead(); 
			 cursor != NULL;
			 cursor = cursor->next
		   )
		{
			if( ((Drawable*)cursor)->onMouseClick( x, y, which ) )
				result = true;
		}
		return result;
	}

	bool DrawList::onMouseDrag( float x, float y, sf::Mouse::Button which )
	{
		if( areChildrenMousable == false )
			return false;

		bool result = false;
		for( const LinkListable *cursor = (LinkListable*)getHead(); 
			 cursor != NULL;
			 cursor = cursor->next
		   )
		{
			if( ((Drawable*)cursor)->onMouseDrag( x, y, which ) )
				result = true;
		}
		return result;
	}