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

        // Set the active window before draw
		App.setActive();

		annotator.update( );

		App.draw( annotator.canvas );

        // draw app
		App.display();
    }

//cleanup
	//TODO: None, yet

    return EXIT_SUCCESS;
}
