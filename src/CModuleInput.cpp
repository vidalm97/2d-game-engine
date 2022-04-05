#include "CModuleInput.h"

bool CModuleInput::HandleInput()
{
	while( SDL_PollEvent(&mEvent) )
		if( mEvent.key.keysym.sym == SDLK_ESCAPE )
			return false;

	return true;
}

bool CModuleInput::Update()
{
	return HandleInput();
}
