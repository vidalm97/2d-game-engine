#include "Modules/CModuleRenderer.h"

#include "CApplication.h"
#include "CComponentBoxCollider.h"
#include "CComponentRenderer.h"
#include "CComponentTransform.h"
#include "Modules/CModuleCamera.h"
#include "Modules/CModuleEditor.h"
#include "Modules/CModuleResourceManager.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

bool CModuleRenderer::Init()
{
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CreateShader( mBackShaderProgram, App->mResourceManager->LoadFile( "shaders/BackVertexShader.txt" ).c_str(),
			App->mResourceManager->LoadFile( "shaders/BackFragmentShader.txt" ).c_str() );
	CreateShader( mShaderProgram, App->mResourceManager->LoadFile( "shaders/VertexShader.txt" ).c_str(),
			App->mResourceManager->LoadFile( "shaders/FragmentShader.txt" ).c_str() );
	CreateShader( mGridShaderProgram, App->mResourceManager->LoadFile( "shaders/GridVertexShader.txt" ).c_str(),
			App->mResourceManager->LoadFile( "shaders/GridFragmentShader.txt" ).c_str() );

	InitFramebuffer( mBackFramebuffer, mBackFramebufferTexture, App->mWindow->GetWidth(), App->mWindow->GetHeight() );
	InitFramebuffer( mSceneFramebuffer, mSceneFramebufferTexture, mSceneWidth, mSceneHeight );
	return InitFramebuffer( mGameFramebuffer, mGameFramebufferTexture, mGameWidth, mGameHeight );
}

bool CModuleRenderer::PreUpdate()
{
	ClearFrameBuffer( mBackFramebuffer, glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	ClearFrameBuffer( mSceneFramebuffer, glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	ClearFrameBuffer( mGameFramebuffer, glm::vec4( 0.0f, 0.3f, 0.6f, 1.0f ) );

	return true;
}

bool CModuleRenderer::Update()
{
	RenderGameObjects( mBackShaderProgram, mBackFramebuffer, App->mCamera->mSceneCamera );
	RenderGameObjects( mShaderProgram, mSceneFramebuffer, App->mCamera->mSceneCamera );
	RenderGameObjects( mShaderProgram, mGameFramebuffer, App->mCamera->mGameCamera );
	RenderColliders();

	return true;
}

bool CModuleRenderer::Clear()
{
	glDeleteFramebuffers( 1, &mBackFramebuffer );
	glDeleteTextures( 1, &mBackFramebufferTexture );

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
	mGameObjects.push_back( CGameObject( ++mNextGOId, new CComponentRenderer( aTextPath ), name ) );

	int r = (mNextGOId & 0x000000FF) >>  0;
	int g = (mNextGOId & 0x0000FF00) >>  8;
	int b = (mNextGOId & 0x00FF0000) >> 16;

	mGOIdColor[mNextGOId] = glm::vec3( r, g, b );
	App->mEditor->SetSelectedGO( mGameObjects.size()-1 );

	return true;
}

void CModuleRenderer::RenderGameObjects( const int& aShaderProgram, const int& aFramebuffer, CCamera* aCamera ) const
{
	glBindFramebuffer( GL_FRAMEBUFFER, aFramebuffer );
	glUseProgram( aShaderProgram );

	glUniformMatrix4fv( glGetUniformLocation( aShaderProgram, "view" ), 1, GL_FALSE, &aCamera->mViewMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( aShaderProgram, "projection" ), 1, GL_FALSE, &aCamera->mProjectionMatrix[0][0] );

	for( const auto& gameObject : mGameObjects )
	{
		if( aShaderProgram == mBackShaderProgram )
			glUniform3f( glGetUniformLocation( aShaderProgram, "color" ), mGOIdColor.at(gameObject.mID).x/255.0f, mGOIdColor.at(gameObject.mID).y/255.0f,
					mGOIdColor.at(gameObject.mID).z/255.0f );

		if( !gameObject.mComponentRenderer || !gameObject.mComponentRenderer->HasTexture() )
			continue;

		glUniformMatrix4fv( glGetUniformLocation( aShaderProgram,"model" ), 1, GL_FALSE, &gameObject.mComponentTransform->mModelMatrix[0][0] );

		gameObject.mComponentRenderer->RenderTexture();
	}

	glUseProgram( 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

bool CModuleRenderer::InitFramebuffer( unsigned int& aFramebuffer, unsigned int& aFramebufferTexture,
		const int aWidth, const int aHeight )
{
	glGenFramebuffers( 1, &aFramebuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, aFramebuffer );

	glGenTextures( 1, &aFramebufferTexture );
	glBindTexture( GL_TEXTURE_2D, aFramebufferTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, aWidth, aHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

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
	ResizeFramebuffer( mBackFramebuffer, mBackFramebufferTexture, aSceneWidth, aSceneHeight );
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

	InitFramebuffer( aFramebuffer, aFramebufferTexture, aWidth, aHeight );
}

void CModuleRenderer::ClearFrameBuffer( const unsigned int aFramebuffer, const glm::vec4& aColor )
{
	glBindFramebuffer( GL_FRAMEBUFFER, aFramebuffer );
	glClearColor( aColor.x, aColor.y, aColor.z, aColor. w );
	glClear( GL_COLOR_BUFFER_BIT );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void CModuleRenderer::CheckSelectedGO( int x, int y )
{
	glBindFramebuffer( GL_READ_FRAMEBUFFER, mBackFramebuffer );
	glReadBuffer( GL_COLOR_ATTACHMENT0 );

	glm::vec3 color;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);

	for( int i = 0; i < mGameObjects.size(); ++i )
	{
		const glm::vec3& GOColor = mGOIdColor[mGameObjects[i].mID];
		if( GOColor.x == int(color.x*256) &&  GOColor.y == int(color.y*256) && GOColor.z == int(color.z*256) )
			App->mEditor->SetSelectedGO(i);
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void CModuleRenderer::RenderColliders()
{
	for( const auto& GO : mGameObjects )
	{
		if( GO.mComponentBoxCollider )
			RenderQuad( GO.mComponentBoxCollider->GetCenter(), GO.mComponentBoxCollider->GetSize() );
	}
}

void CModuleRenderer::RenderQuad( const glm::vec2& aCenter, const glm::vec2& aSize )
{
	std::vector<float> vertices =
	{
		aCenter.x - aSize.x/2, aCenter.y - aSize.y/2, 0.0f,
		aCenter.x - aSize.x/2, aCenter.y + aSize.y/2, 0.0f,
		aCenter.x + aSize.x/2, aCenter.y + aSize.y/2, 0.0f,
		aCenter.x + aSize.x/2, aCenter.y - aSize.y/2, 0.0f,

		aCenter.x - aSize.x/2, aCenter.y - aSize.y/2, 0.0f,
		aCenter.x + aSize.x/2, aCenter.y - aSize.y/2, 0.0f,
		aCenter.x - aSize.x/2, aCenter.y + aSize.y/2, 0.0f,
		aCenter.x + aSize.x/2, aCenter.y + aSize.y/2, 0.0f,
	};

	unsigned int VBO;
	glGenBuffers( 1, &VBO );
	unsigned int VAO;
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 );
	glEnableVertexAttribArray( 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, mSceneFramebuffer );

	glUseProgram( mGridShaderProgram );
	glUniformMatrix4fv( glGetUniformLocation( mGridShaderProgram, "view" ), 1, GL_FALSE, &App->mCamera->mSceneCamera->mViewMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mGridShaderProgram, "projection" ), 1, GL_FALSE, &App->mCamera->mSceneCamera->mProjectionMatrix[0][0] );
	glUniform3f( glGetUniformLocation( mGridShaderProgram, "color" ), 0.0f, 1.0f, 0.0f );

	glBindVertexArray( VAO );
	glLineWidth(2.0f);
	glDrawArrays( GL_LINES, 0, 8 );
	glLineWidth(1.0f);
	glBindVertexArray( 0 );

	glUseProgram( 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
