#include "CModuleWindow.h"

CModuleWindow::CModuleWindow( const int aWidth, const int aHeight ) : mWidth(aWidth), mHeight(aHeight)
{
}

bool CModuleWindow::Init()
{
	SDL_Init(SDL_INIT_VIDEO);
	mWindow = SDL_CreateWindow( "2D Game Engine", 0, 0, 640, 480, SDL_WINDOW_OPENGL );
	return mWindow;
}

bool CModuleWindow::Update()
{
	SDL_Event event;

	while( SDL_PollEvent(&event) )
		if( event.type == SDL_QUIT )
			return false;

	return true;
}

bool CModuleWindow::Clear()
{
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	return true;
}
