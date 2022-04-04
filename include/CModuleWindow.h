#ifndef CMODULEWINDOW__H
#define CMODULEWINDOW__H

#include "AModule.h"

#include "SDL.h"

class CModuleWindow : public AModule
{
public:
	CModuleWindow( const int aWidth, const int aHeight );
	~CModuleWindow() = default;

	bool Init() override;
	bool Update() override;
	bool Clear() override;

private:
	int mWidth, mHeight;
	SDL_Window *mWindow = nullptr;
};

#endif
