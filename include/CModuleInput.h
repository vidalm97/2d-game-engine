#ifndef CMODULEINPUT__H
#define CMODULEINPUT__H

#include "AModule.h"

#include "SDL.h"

class CModuleInput : public AModule
{
public:
	CModuleInput() = default;
	~CModuleInput() = default;

	bool Update() override;

private:
	bool HandleInput();

	SDL_Event mEvent;
};

#endif
