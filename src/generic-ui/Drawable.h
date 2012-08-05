#ifndef DRAWLIST_IS_HONEST_TO_A_FAULT
#define DRAWLIST_IS_HONEST_TO_A_FAULT

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <string>

#include "Mousable.h"

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
					public sf::Drawable,
					public IsMousable
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

	virtual bool isPointInside( float x, float y )		{ return false; }

	virtual void onMouseOver( MouseEvent &e )			{ return; }
	virtual void onMouseOut( MouseEvent &e )			{ return; }
	virtual void onMouseClick( MouseEvent &e )			{ return; }
	virtual void onMouseDrag( MouseEvent &e )			{ return; }
	virtual void onMouseDragRelease( MouseEvent &e )	{ return; }

	virtual float getHeight() = 0;
	virtual sf::FloatRect getBounds() = 0;
	virtual sf::FloatRect getClickBounds() = 0;
	
	// mouseable stuff, for passing down the list
	//TODO: would rather have this hidden away, not part of the Drawable interface
	virtual void internal_onMouseMove( MouseEvent e )
	{	//printf( "%s, Mousing\n", name.c_str() );
		this->internal_mousable.internal_onMouseMove( e );	}
	virtual void internal_onMouseDown( MouseEvent e )
	{	this->internal_mousable.internal_onMouseDown( e );	}
	virtual void internal_onMouseUp( MouseEvent e )
	{	this->internal_mousable.internal_onMouseUp( e );	}

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

	//void onMouseOver( MouseEvent &e );
	//void onMouseOut( MouseEvent &e );
	//void onMouseClick( MouseEvent &e );
	//void onMouseDrag( MouseEvent &e );
	//void onMouseDragRelease ( MouseEvent &e );

	virtual void internal_onMouseMove( MouseEvent e );
	virtual void internal_onMouseDown( MouseEvent e );
	virtual void internal_onMouseUp( MouseEvent e );
	
	sf::FloatRect getBounds();
	sf::FloatRect getClickBounds();

	float getHeight();
};


// quick mix-in of SFML classes, to avoid wrapper dilemma

class Sprite :	public Drawable
{
public:
	sf::Sprite sf;
	sf::Texture image;

	float width, height;

	Sprite()
	{	width = 100;
		height = 100;
	}

//	bool isPointInside( float x, float y )
//	{	return sf.getGlobalBounds().contains( sf::Vector2f( x, y ) );	}
	
	void setSize( float w, float h )
	{
		width = w;
		height = h;
		
		// rescale image
		sf.setScale(  1, 1 );
		sf::FloatRect size = sf.getGlobalBounds();
		float wScale = width / size.width;
		float hScale = height / size.height;
		sf.setScale(  wScale, hScale );
	}

	float getHeight()			{	return sf.getLocalBounds().height;	}
	sf::FloatRect getBounds()	{	return sf.getLocalBounds();			}
	sf::FloatRect getClickBounds()	{ return getBounds();				}

	bool loadImage( char * filename )
	{
		if( !image.loadFromFile( filename ) )
			return false;
		
		sf.setTexture( image );

		sf::FloatRect size = sf.getGlobalBounds();
		float wScale = width / size.width;
		float hScale = height / size.height;
		sf.setScale(  wScale, hScale );
		return true;
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

	float getHeight()			{	return sf.getLocalBounds().height;	}
	sf::FloatRect getBounds()	{	return sf.getLocalBounds();			}
	sf::FloatRect getClickBounds()	{ return getBounds();				}

	void setSize( int width, int height )
	{
		sf.setSize( sf::Vector2f( width, height ) );
	}

	void setColor( int r, int g, int b )
	{
		sf.setFillColor( sf::Color( r, g, b ) );
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

//	bool isPointInside( float x, float y )
//	{	return sf.getGlobalBounds().contains( sf::Vector2f( x, y ) );	}

	float getHeight()			{	return sf.getLocalBounds().height;	}
	sf::FloatRect getBounds()	{	return sf.getLocalBounds();			}
	sf::FloatRect getClickBounds()	{ return getBounds();				}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{	states.transform.combine( this->getTransform() );	target.draw( sf, states );	}
	
};

class Button : public DrawList
{
public:

	Drawable *off, *over, *down;

	Button()
	{
		//over->visible = false;
		//down->visible = false;

		//off->visible = true;
		name = "Button";
	}


	void onMouseOver( MouseEvent &e )
	{
		printf( "%s, mouse over\n", name.c_str() );
		down->visible = false;
		off->visible = false;

		over->visible = true;
	}	
	
	void onMouseOut( MouseEvent &e )
	{
		printf( "%s, mouse out\n", name.c_str() );
		over->visible = false;
		down->visible = false;

		off->visible = true;
	}

	void onMouseClick( MouseEvent &e )
	{
		printf( "%s, mouse click\n", name.c_str() );
		over->visible = false;
		off->visible = false;

		down->visible = true;
	}

};



#endif