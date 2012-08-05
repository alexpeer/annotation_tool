#include "../TheAnnotator.h"
#include "Buttons.h"


	void Button_Play::onMouseClick( MouseEvent &e )
	{
		if( theBoss->isPlaying )
			theBoss->stop();
		else
			theBoss->play();
	}