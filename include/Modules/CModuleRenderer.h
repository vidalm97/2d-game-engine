#ifndef CMODULERENDERER__H
#define CMODULERENDERER__H

#include "AModule.h"
#include "CGameObject.h"

#include <vector>
#include <string>

class CTexture;

class CModuleRenderer : public AModule
{
public:
	CModuleRenderer() = default;
	~CModuleRenderer() = default;

	bool Init() override;
	bool PreUpdate() override;
	bool Update() override;
	bool Clear() override;

	bool GenerateGameObjectWithTexture( const std::string& aTextPath );
	void RenderGameObjects() const;

	unsigned int mShaderProgram;
	unsigned int mGridShaderProgram;

	std::vector<CGameObject> mGameObjects;

private:
	bool CreateShader( unsigned int& aShaderProgram, const char* aVertexShaderSource, const char* aFragmentShaderSource ) const;
};

#endif
