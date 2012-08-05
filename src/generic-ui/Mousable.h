#ifndef MOUSABLE_WOULD_MAKE_A_TERRIBLE_ELEPHANT
#define MOUSABLE_WOULD_MAKE_A_TERRIBLE_ELEPHANT

#include <SFML/Graphics.hpp>

// just to wrap the SFML location stuff; eventually, replace with own
class Transformable : public sf::Transformable
{	//virtual sf::Transform getTransform() = 0; // for now, comes from sf::Transformable
};

class MouseEvent
{
public:
	sf::Vector2f mousePos_global;
	sf::Transform transform;
	sf::Transform transform_inverse;

	sf::Vector2f mouseDownPoint_local; //only contains info 

	bool left, right, middle;

	sf::Vector2f getPos_local()
	{
		return transform_inverse.transformPoint( mousePos_global );
	}

	MouseEvent(	sf::Vector2f mousePos_global, 
				bool isLeftDown, bool isRightDown, bool isMiddleDown, 
				sf::Transform transform )
	{
		this->mousePos_global = mousePos_global;
		this->transform = transform;
		this->transform_inverse = transform.getInverse();

		left = isLeftDown;
		right = isRightDown;
		middle = isMiddleDown;
	}
	
	MouseEvent(	sf::Vector2f mousePos_global, 
				sf::Mouse::Button which, 
				sf::Transform transform )
	{
		left		= ( which == sf::Mouse::Button::Left )	? true : false;
		right		= ( which == sf::Mouse::Button::Right )	? true : false;
		middle	= ( which == sf::Mouse::Button::Middle )	? true : false;

		this->mousePos_global = mousePos_global;
		this->transform = transform;
		this->transform_inverse = transform.getInverse();
	}
	
	MouseEvent(	sf::Vector2f mousePos_global, 
				sf::Transform transform )
	{
		left = false;
		right = false;
		middle = false;

		this->mousePos_global = mousePos_global;
		this->transform = transform;
		this->transform_inverse = transform.getInverse();
	}
};

class IsMousable;

class MouseStuff
{
public:

	MouseStuff()
	{
		wasMouseOver = false;
		wasDragging = false;
		wasDownIn = false;
	}

	IsMousable *chief;

	bool wasMouseOver, wasDragging, wasDownIn;
	sf::Vector2f mouseDownPoint_local;

	virtual void internal_onMouseMove( MouseEvent e );
	virtual void internal_onMouseDown( MouseEvent e );
	virtual void internal_onMouseUp( MouseEvent e );
};

class IsMousable : public Transformable
{
public:
	MouseStuff internal_mousable;

	virtual sf::FloatRect getClickBounds() = 0;
		//NOTE: bounds should be returned in local coords

	IsMousable()
	{
		internal_mousable.chief = this;
	}

	virtual void onMouseOver( MouseEvent &e )			= 0;
	virtual void onMouseOut( MouseEvent &e )			= 0;
	virtual void onMouseClick( MouseEvent &e )			= 0;
	virtual void onMouseDrag( MouseEvent &e )			= 0;
	virtual void onMouseDragRelease( MouseEvent &e )	= 0;

};


#endif