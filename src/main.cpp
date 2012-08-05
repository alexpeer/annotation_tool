#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>

#include "generic-ui/ui.h"
#include "Timeline.h"
#include "Video.h"
#include "Stream.h"

#include "TheAnnotator.h"

#include <cstdio>  // For ddebug / stdout


int main()
{
	int viewWidth = 1000;
	int viewHeight = 800;
 
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(viewWidth, viewHeight, 32), "Annotation Tool");
	
   // Create a clock for measuring time elapsed
    sf::Clock Clock;
	Clock.restart();
	
	TheAnnotator annotator;
	annotator.resize( viewWidth, viewHeight );

	/*
	Timeline timeline;

	Video vid;
	//vid.openFile( "bubbles.avi" );
	//vid.openFile( "test-bubbles.avi" );
	vid.openFile( "test-eye_tracker.avi ");
	//vid.openFile( "sample_for_annotator_00000.avi" );
	//vid.openFile( "" );
	vid.play();
	// may need to prep video by decoding with mencoder first, command line looks like:  
	//    mencoder sample.avi -ovc raw -vf format=i420 -oac copy -o out.avi

	// Gorillaz-Stylo-Alex-Metric-Remix.mp3

	//TODO: add stream to timeline instead, handle this internally
	timeline.set_endTime( vid.duration );
	*/

	annotator.addVideoStream( "test-eye_tracker.avi" );
	//annotator.addAudioStream( "Gorillaz-Stylo-Alex-Metric-Remix.mp3" );
	//annotator.addAudioStream( "test-eye_tracker.avi" );
	annotator.addAudioStream( "Gorillaz-Stylo-Alex-Metric-Remix.wav" );
	
	//annotator.play();

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


			// mouse events
// mouse events
			if( Event.type == sf::Event::MouseButtonPressed )
			{
				//printf( "sf::Event mouseDown\n" );
				//do mouseDown
				MouseEvent e(	sf::Vector2f( Event.mouseButton.x, Event.mouseButton.y ),
								Event.mouseButton.button,
								annotator.canvas.getTransform()
							);
				annotator.canvas.internal_onMouseDown( e );

			}
			if( Event.type == sf::Event::MouseButtonReleased )
			{
				//printf( "sf::Event mouseUp\n" );

				//do mouseUp
				MouseEvent e(	sf::Vector2f( Event.mouseButton.x, Event.mouseButton.y ),
								Event.mouseButton.button,
								annotator.canvas.getTransform()
							);
				annotator.canvas.internal_onMouseUp( e );
			}


			if( Event.type == sf::Event::MouseMoved )
			{	
				//printf( "sf::Event mouseMoved\n" );
				//do mouseMove
				MouseEvent e(	sf::Vector2f( Event.mouseMove.x, Event.mouseMove.y ),
								annotator.canvas.getTransform()
							);
				annotator.canvas.internal_onMouseMove( e );
			}

        }

		
//		bool leftButtonDown		= sf::Mouse::isButtonPressed(sf::Mouse::Left);
//		unsigned int mouseX		= sf::Mouse::getPosition().x;
//        unsigned int mouseY		= sf::Mouse::getPosition().y;
//		bool isSpacePressed		= sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
//		bool isAPressed			= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
//		bool isSPressed			= sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		
/*		if(isSpacePressed)
		{
			if( annotator.isPlaying() )
				annotator.stop();
			else
				annotator.play();
		}
*/

        // Set the active window before draw
		App.setActive();

//		sf::CircleShape circle;
//		int radius = 20;
//		circle.setRadius(radius);
//		circle.setOutlineColor(sf::Color::Red);
//		circle.setOutlineThickness(5);
//		sf::Vector2i mousePos = sf::Mouse::getPosition(App);
//		circle.setPosition(mousePos.x-radius/2, mousePos.y-radius/2);

		//App.draw( circle );

		//App.draw( scene );

//		BoxWithText box( 300 ,50, 300, 100);
//		box.name = "box";
//		box.words.sf.setString( "Hallo" );

//		App.draw( box );

//		vid.update();
//		App.draw( vid );

//		timeline.update_cursor( vid.location );

//		App.draw( timeline );
		//TODO: make "Interface" class to encapsulate main drawing / UI logic
		
		//do mouse stuff


		annotator.update( );

		App.draw( annotator.canvas );

        // draw app
		App.display();
    }

//cleanup
	//TODO: None, yet

    return EXIT_SUCCESS;
}
