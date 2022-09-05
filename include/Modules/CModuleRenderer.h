#ifndef CMODULERENDERER__H
#define CMODULERENDERER__H

#include "AModule.h"
#include "CGameObject.h"

#include "glm.hpp"

#include <map>
#include <string>
#include <vector>

class CTexture;
class CCamera;

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
	void RenderGameObjects( const int& aShaderProgram, const int& aFramebuffer, CCamera* aCamera ) const;

	void ResizeSceneFramebuffer( const int aSceneWidth, const int aSceneHeight );
	void ResizeGameFramebuffer( const int aGameWidth, const int aGameHeight );

	unsigned int mShaderProgram;
	unsigned int mGridShaderProgram;
	unsigned int mBackShaderProgram;

	unsigned int mSceneFramebuffer;
	unsigned int mSceneFramebufferTexture;

	unsigned int mGameFramebuffer;
	unsigned int mGameFramebufferTexture;

	// Just used for mouse picking
	unsigned int mBackFramebuffer;
	unsigned int mBackFramebufferTexture;

	void CheckSelectedGO( int x, int y );
	std::vector<CGameObject> mGameObjects;

	float mSceneWidth = 1.0f;
	float mSceneHeight = 1.0f;

	float mSceneViewportXPos = 0.0f;
	float mSceneViewportYPos = 0.0f;
	void ResizeFramebuffer( unsigned int& aFramebuffer, unsigned int& aFramebufferTexture, const int aWidth, const int aHeight );
private:
	unsigned int mNextGOId = 0;
	std::map<unsigned int, glm::vec3> mGOIdColor;

	bool InitFramebuffer( unsigned int& aFramebuffer, unsigned int& aFramebufferTexture, const int aWidth, const int aHeight );
	void ClearFrameBuffer( const unsigned int aFramebuffer, const glm::vec4& aColor );

	bool CreateShader( unsigned int& aShaderProgram, const char* aVertexShaderSource, const char* aFragmentShaderSource ) const;

	float mGameWidth = 1.0f;
	float mGameHeight = 1.0f;
};

#endif
