#ifndef DRAWLIST_IS_HONEST_TO_A_FAULT
#define DRAWLIST_IS_HONEST_TO_A_FAULT

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <string>


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
		add( &thing );
	}

	void remove( LinkListable *thing );

	void empty( );

	LinkListable* getHead() const;

private:
	LinkListable *head, *tail;
	int size;
};

class Drawable :	public LinkListable, 
					public sf::Drawable, public sf::Transformable
{
friend class DrawList;
public:
	bool visible;
	std::string name;

	Drawable()
	{
		visible = true;
		name = "";
	}

	virtual bool isPointInside( float x, float y )  { return false; }
	virtual bool onMouseMove( float x, float y )  { return false; }
	virtual bool onMouseClick( float x, float y, sf::Mouse::Button which )  { return false; }
	virtual bool onMouseDrag( float x, float y, sf::Mouse::Button which )  {	return false; }

	virtual float getHeight() = 0;

//private:
	sf::Vector2f getParentPosition();
};


class DrawList : public Drawable, public LinkList
{

public:

	DrawList()
	{
		name = "Drawlist";
		visible = true;
		areChildrenMousable = true;	
	}

	bool areChildrenMousable;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool isPointInside( float x, float y );
	bool onMouseMove( float x, float y );
	bool onMouseClick( float x, float y, sf::Mouse::Button which );
	bool onMouseDrag( float x, float y, sf::Mouse::Button which );

	float getHeight();
};


// quick mix-in of SFML classes, to avoid wrapper dilemma

class Sprite :	public Drawable
{
public:
	sf::Sprite sf;

	bool isPointInside( float x, float y )
	{	return sf.getGlobalBounds().contains( sf::Vector2f( x, y ) );	}
	
	float getHeight()
	{
		return sf.getLocalBounds().height;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{	states.transform.combine( this->getTransform() );	target.draw( sf, states );	}

};

class RectangleShape :	public Drawable
{
public:
	sf::RectangleShape sf;
	
//	void setSize(const sf::Vector2f &size)
//	{	sf.setSize( size );	}

	bool isPointInside( float x, float y )
	{	
		sf::Vector2f parentPos = this->getParentPosition();
		sf::FloatRect bounds = sf.getGlobalBounds();

		return bounds.contains( sf::Vector2f( x - parentPos.x, y - parentPos.y ) );	
	}

	float getHeight()
	{
		return sf.getLocalBounds().height;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{	states.transform.combine( this->getTransform() );	target.draw( sf, states );	}

};

class Text : public Drawable
{
public:
	sf::Text sf;

//	void setString( const String &string )
//	{	sf.setString( string );	}

	bool isPointInside( float x, float y )
	{	return sf.getGlobalBounds().contains( sf::Vector2f( x, y ) );	}
	
	float getHeight()
	{
		return sf.getLocalBounds().height;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{	states.transform.combine( this->getTransform() );	target.draw( sf, states );	}
	
};

#endif