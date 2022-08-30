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
	void ResizeSceneFramebuffer( const int aSceneWidth, const int aSceneHeight );
	void ResizeGameFramebuffer( const int aGameWidth, const int aGameHeight );

	unsigned int mShaderProgram;
	unsigned int mGridShaderProgram;

	unsigned int mSceneFramebuffer;
	unsigned int mSceneFramebufferTexture;
	unsigned int mSceneDepthAttachment;

	unsigned int mGameFramebuffer;
	unsigned int mGameFramebufferTexture;
	unsigned int mGameDepthAttachment;

	std::vector<CGameObject> mGameObjects;

private:
	bool InitSceneFramebuffer();
	bool InitGameFramebuffer();
	bool CreateShader( unsigned int& aShaderProgram, const char* aVertexShaderSource, const char* aFragmentShaderSource ) const;

	float mSceneWidth = 1.0f;
	float mSceneHeight = 1.0f;
	float mGameWidth = 1.0f;
	float mGameHeight = 1.0f;
};

#endif
