#include "Modules/CModuleRenderer.h"

#include "CApplication.h"
#include "CComponentRenderer.h"
#include "CComponentTransform.h"
#include "Modules/CModuleCamera.h"
#include "Modules/CModuleResourceManager.h"
#include "Modules/CModuleWindow.h"

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

bool CModuleRenderer::Init()
{
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CreateShader( mShaderProgram, App->mResourceManager->LoadFile( "shaders/VertexShader.txt" ).c_str(),
			App->mResourceManager->LoadFile( "shaders/FragmentShader.txt" ).c_str() );
	CreateShader( mGridShaderProgram, App->mResourceManager->LoadFile( "shaders/GridVertexShader.txt" ).c_str(),
			App->mResourceManager->LoadFile( "shaders/GridFragmentShader.txt" ).c_str() );

	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"view" ), 1, GL_FALSE, &App->mCamera->mSceneCamera->mViewMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"projection" ), 1, GL_FALSE, &App->mCamera->mSceneCamera->mProjectionMatrix[0][0] );

	InitFramebuffer( mSceneFramebuffer, mSceneFramebufferTexture );
	return InitFramebuffer( mGameFramebuffer, mGameFramebufferTexture );
}

bool CModuleRenderer::PreUpdate()
{
	ClearFrameBuffer( mSceneFramebuffer, glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
	ClearFrameBuffer( mGameFramebuffer, glm::vec4( 0.0f, 0.3f, 0.6f, 1.0f ) );

	return true;
}

bool CModuleRenderer::Update()
{
	RenderGameObjects( mSceneFramebuffer, App->mCamera->mSceneCamera );
	RenderGameObjects( mGameFramebuffer, App->mCamera->mGameCamera );

	return true;
}

bool CModuleRenderer::Clear()
{
	glDeleteFramebuffers( 1, &mSceneFramebuffer );
	glDeleteTextures( 1, &mSceneFramebufferTexture );

	glDeleteFramebuffers( 1, &mGameFramebuffer );
	glDeleteTextures( 1, &mGameFramebufferTexture );

	return true;
}

bool CModuleRenderer::CreateShader( unsigned int& aShaderProgram, const char* aVertexShaderSource, const char* aFragmentShaderSource ) const
{
	unsigned int vertexShader;
	vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader, 1, &aVertexShaderSource, NULL );
	glCompileShader( vertexShader );

	int success;
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
	if( !success )
		return false;

	unsigned int fragmentShader;
	fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShader, 1, &aFragmentShaderSource, NULL );
	glCompileShader( fragmentShader );

	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
	if( !success )
		return false;

	aShaderProgram = glCreateProgram();
	glAttachShader( aShaderProgram, vertexShader );
	glAttachShader( aShaderProgram, fragmentShader );
	glLinkProgram( aShaderProgram );

	glGetProgramiv( aShaderProgram, GL_LINK_STATUS, &success );
	if(!success)
		return false;

	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );

	return true;
}

bool CModuleRenderer::GenerateGameObjectWithTexture( const std::string& aTextPath )
{
	const std::string name = "New GameObject" + std::to_string(mGameObjects.size());
	mGameObjects.push_back( CGameObject( new CComponentRenderer( aTextPath ), name ) );

	return true;
}

void CModuleRenderer::RenderGameObjects( const int& aFramebuffer, CCamera* aCamera ) const
{
	glBindFramebuffer( GL_FRAMEBUFFER, aFramebuffer );
	glUseProgram( mShaderProgram );

	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram, "view" ), 1, GL_FALSE, &aCamera->mViewMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram, "projection" ), 1, GL_FALSE, &aCamera->mProjectionMatrix[0][0] );

	for( const auto& gameObject : mGameObjects )
	{
		if( !gameObject.mComponentRenderer || !gameObject.mComponentRenderer->HasTexture() )
			continue;

		glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"model" ), 1, GL_FALSE, &gameObject.mComponentTransform->mModelMatrix[0][0] );

		gameObject.mComponentRenderer->RenderTexture();
	}

	glUseProgram( 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

bool CModuleRenderer::InitFramebuffer( unsigned int& aFramebuffer, unsigned int& aFramebufferTexture )
{
	glGenFramebuffers( 1, &aFramebuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, aFramebuffer );

	glGenTextures( 1, &aFramebufferTexture );
	glBindTexture( GL_TEXTURE_2D, aFramebufferTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mSceneWidth, mSceneHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glBindTexture( GL_TEXTURE_2D, 0 );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aFramebufferTexture, 0 );

	if( glCheckFramebufferStatus( GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		return true;
	}

	return false;
}

void CModuleRenderer::ResizeSceneFramebuffer( const int aSceneWidth, const int aSceneHeight )
{
	mSceneWidth = aSceneWidth;
	mSceneHeight = aSceneHeight;

	ResizeFramebuffer( mSceneFramebuffer, mSceneFramebufferTexture, aSceneWidth, aSceneHeight );
}

void CModuleRenderer::ResizeGameFramebuffer( const int aGameWidth, const int aGameHeight )
{
	mGameWidth = aGameWidth;
	mGameHeight = aGameHeight;

	ResizeFramebuffer( mGameFramebuffer, mGameFramebufferTexture, aGameWidth, aGameHeight );
}

void CModuleRenderer::ResizeFramebuffer( unsigned int& aFramebuffer, unsigned int& aFramebufferTexture, const int aWidth, const int aHeight )
{
	glDeleteFramebuffers( 1, &aFramebuffer );
	glDeleteTextures( 1, &aFramebufferTexture );

	InitFramebuffer( aFramebuffer, aFramebufferTexture );
}

void CModuleRenderer::ClearFrameBuffer( const unsigned int aFrameBuffer, const glm::vec4& aColor )
{
	glBindFramebuffer( GL_FRAMEBUFFER, aFrameBuffer );
	glClearColor( aColor.x, aColor.y, aColor.z, aColor. w );
	glClear( GL_COLOR_BUFFER_BIT );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
