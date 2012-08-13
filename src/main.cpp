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

	//annotator.addVideoStream( "sample_for_annotator_00000.avi" );
	annotator.addVideoStream( "test-eye_tracker.avi" );
	//annotator.addAudioStream( "Gorillaz-Stylo-Alex-Metric-Remix.mp3" );
	//annotator.addAudioStream( "test-eye_tracker.avi" );
	AudioStream * testAudio = annotator.addAudioStream( "test-eye_tracker.wav" );

	//TODO: add setting offset to config file load routine
	testAudio->setOffset( 24000 );
	
	EventStream * testText = annotator.addEventStream( "test-eye_tracker_test_text.csv" );
	testText->displayTarget = &(annotator.text_area);
		//TODO: need to set a default in annotator, use it on stream create; then, use UI (drag / drop? config dialouge?) to set draw targets
	
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
            {
				glViewport(0, 0, Event.size.width, Event.size.height);
				annotator.resize( Event.size.width, Event.size.height );
			}


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
