#include "Drawable.h";

//::::::::: LinkListable

	LinkListable::LinkListable()
	{
		owner = NULL;
		//parent = NULL;
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
			owner->head == NULL;
		if( owner->tail == this )
			owner->tail == NULL;
		owner->size--;

		this->owner = NULL;
	}

	void LinkListable::append( LinkListable *thing )
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
