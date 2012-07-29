#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>

#include "generic-ui/ui.h"
#include "Timeline.h"
#include "Video.h"

#include <cstdio>  // For ddebug / stdout


int main()
{
	int viewWidth = 1000;
	int viewHeight = 800;
 
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(viewWidth, viewHeight, 32), "Cube Brain");
	
   // Create a clock for measuring time elapsed
    sf::Clock Clock;	
	Clock.restart();
	
	Video vid;
	//vid.openFile( "bubbles.avi" );
	//vid.openFile( "test-bubbles.avi" );
	vid.openFile( "test-eye_tracker.avi ");
	//vid.openFile( "sample_for_annotator_00000.avi" );
	// mencoder in.avi -ovc raw -vf format=i420 -o out.avi 
	//vid.openFile( "" );
	vid.play();
	// mencoder sample.avi -ovc raw -vf format=i420 -oac copy -o out.avi

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
		box.name = "box";
		box.words.sf.setString( "Hallo" );

		App.draw( box );

		vid.update();
		App.draw( vid );

		Timeline timeline;
		App.draw( timeline );
		//TODO: make "Interface" class to encapsulate main drawing / UI logic

        // draw app
		App.display();
    }

//cleanup
	//TODO: None, yet

    return EXIT_SUCCESS;
}
