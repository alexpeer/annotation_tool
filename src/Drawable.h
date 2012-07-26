#ifndef DRAWLIST_IS_HONEST_TO_A_FAULT
#define DRAWLIST_IS_HONEST_TO_A_FAULT

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class LinkListable
{
friend class LinkList;
public:
	//LinkListable *parent;
	LinkListable *next, *prev;

	LinkList *owner;

	LinkListable();

	void remove();

	void append( LinkListable *thing );

	void prepend( LinkListable *thing );
};

class LinkList
{
friend class LinkListable;
public:
	LinkList();

	void add( LinkListable *thing );

	void add( LinkListable &thing )
	{
//		add( &thing );
	}

	void remove( LinkListable *thing );

	void empty( );

	LinkListable* getHead() const;

private:
	LinkListable *head, *tail;
	int size;
};

class HasVisibility
{
public:
	bool visible;

	HasVisibility()
	{
		visible = true;
	}
};

class Drawable :	public LinkListable, public HasVisibility, 
					public sf::Drawable, public sf::Transformable
{
friend class DrawList;

};

/*
class Drawable_wrapper : public Drawable
{
public:
	Drawable_wrapper( sf::Drawable * delicious )
	{
		nugat = delicious;
	}
	
	Drawable_wrapper( )
	{
		nugat = NULL;
	}

	void wrap( sf::Drawable * delicious )
	{
		nugat = delicious;
	}

private:
	sf::Drawable * nugat;

	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw( *nugat, states );
	}
};
*/

class DrawList : public Drawable, public LinkList
{

public:

//	void add( sf::Drawable *thing )
//	{
//		Drawable_wrapper_dynamic * wrap = new Drawable_wrapper( thing );
//		//TODO: maintain list of dynamically allocated wrappers to be reaped on remove
//		LinkList::add( &wrap );
//	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


// quick mix-in of SFML classes, to avoid wrapper dilemma

class Sprite :	public LinkListable, public HasVisibility,
				public sf::Sprite
{

};

class RectangleShape :	public LinkListable, public HasVisibility,
						public sf::RectangleShape
{

};

class Text :	public LinkListable, public HasVisibility,
				public sf::Text
{

};

#endif