#include "CModuleRenderer.h"

#include "CApplication.h"
#include "CModuleWindow.h"

bool CModuleRenderer::Init()
{
	mRenderer = SDL_CreateRenderer( App->mWindow->mWindow, -1, SDL_RENDERER_ACCELERATED );
	return mRenderer;
}

bool CModuleRenderer::Update()
{
	SDL_SetRenderDrawColor( mRenderer, 255, 0, 0, 255 );
	SDL_RenderClear( mRenderer );

    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = 20;
    r.h = 20;

    SDL_SetRenderDrawColor( mRenderer, 0, 0, 255, 255 );
    SDL_RenderFillRect( mRenderer, &r );
    SDL_RenderPresent(mRenderer);

	return true;
}

bool CModuleRenderer::Clear()
{
	SDL_DestroyRenderer(mRenderer);
	return true;
}
