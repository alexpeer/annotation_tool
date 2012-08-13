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

	bool left, right, middle; //these are maintained in the MouseStuff, so the internal_ functions may not have accurate info from the event obj (they build it), but onX funcs should
	sf::Mouse::Button button;

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
		button = which;

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

		button = sf::Mouse::Button::ButtonCount; // as good as NULL; TODO: replace sf::Mouse

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

		leftDown = false;
		middleDown = false;
		rightDown = false;
	}

	IsMousable *chief;

	bool wasMouseOver, wasDragging, wasDownIn;
	bool leftDown, middleDown, rightDown;
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