#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>

#include "BoxWithText.h"

#include <cstdio>  // For ddebug / stdout


int main()
{
	int viewWidth = 800;
	int viewHeight = 800;
 
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(viewWidth, viewHeight, 32), "Cube Brain");
	
   // Create a clock for measuring time elapsed
    sf::Clock Clock;	
	Clock.restart();

    // Start draw loop
	while (App.isOpen())
    {
		float elapsedTime = Clock.getElapsedTime().asMilliseconds();
        Clock.restart();

		App.clear();

        // Process events
        sf::Event Event;
		while (App.pollEvent(Event))
        {
            // Close window : exit
			if (Event.type == sf::Event::Closed)
				App.close();

            // Escape key : exit
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                App.close();

            // Resize event : adjust viewport
            if (Event.type == sf::Event::Resized)
                glViewport(0, 0, Event.size.width, Event.size.height);
        }

		
		bool leftButtonDown		= sf::Mouse::isButtonPressed(sf::Mouse::Left);
		unsigned int mouseX		= sf::Mouse::getPosition().x;
        unsigned int mouseY		= sf::Mouse::getPosition().y;
		bool isSpacePressed		= sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
		bool isAPressed			= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		bool isSPressed			= sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		
		if(isSpacePressed)
		{
			;
		}


        // Set the active window before draw
		App.setActive();

		sf::CircleShape circle;
		int radius = 20;
		circle.setRadius(radius);
		circle.setOutlineColor(sf::Color::Red);
		circle.setOutlineThickness(5);
		sf::Vector2i mousePos = sf::Mouse::getPosition(App);
		circle.setPosition(mousePos.x-radius/2, mousePos.y-radius/2);

		//App.draw( circle );

		//App.draw( scene );

		BoxWithText box( 300 ,50, 300, 100);

		App.draw( box );

        // draw app
		App.display();
    }

//cleanup
	//TODO: None, yet

    return EXIT_SUCCESS;
}
