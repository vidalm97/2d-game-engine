#ifndef CMODULERENDERER__H
#define CMODULERENDERER__H

#include "AModule.h"

#include "SDL.h"

class CModuleRenderer : public AModule
{
public:
	CModuleRenderer() = default;
	~CModuleRenderer() = default;

	bool Init() override;
	bool Update() override;
	bool Clear() override;

private:
	SDL_Renderer *mRenderer = nullptr;
};

#endif
