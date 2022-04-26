#ifndef CMODULERENDERER__H
#define CMODULERENDERER__H

#include "AModule.h"
#include "CTexture.h"

#include "SDL.h"

#include <vector>
#include <string>

class CTexture;

class CModuleRenderer : public AModule
{
public:
	CModuleRenderer() = default;
	~CModuleRenderer() = default;

	bool Init() override;
	bool Update() override;
	bool Clear() override;

	bool GenerateTexture( const std::string& aTextPath );
	void RenderTextures() const;

	unsigned int mShaderProgram;

private:
	SDL_Renderer *mRenderer = nullptr;
	std::vector<CTexture> mTextures;
	unsigned int mVBO;
	unsigned int mVAO;
};

#endif
