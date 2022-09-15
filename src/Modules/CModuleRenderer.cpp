#include "Modules/CModuleRenderer.h"

#include "CApplication.h"
#include "CComponentAnimation.h"
#include "CComponentBoxCollider.h"
#include "CComponentRenderer.h"
#include "CComponentTransform.h"
#include "Modules/CModuleCamera.h"
#include "Modules/CModuleEditor.h"
#include "Modules/CModuleResourceManager.h"
#include "Modules/CModuleTime.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

CModuleRenderer::CModuleRenderer() :
	mGizmo( false )
{
}

bool CModuleRenderer::Init()
{
	mGizmo.AttachAxisTextures( "../engine_resources/icons/greenArrow.png", "../engine_resources/icons/blueArrow.png" );
	mGizmo.SetSize( glm::vec2( 1.5f, 1.5f ) );

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
	ClearFrameBuffer( mSceneFramebuffer, glm::vec4( 0.3f, 0.3f, 0.3f, 1.0f ) );
	ClearFrameBuffer( mGameFramebuffer, glm::vec4( 0.0f, 0.3f, 0.6f, 1.0f ) );

	return true;
}

bool CModuleRenderer::Update()
{
	RenderGameObjects( mBackShaderProgram, mBackFramebuffer, App->mCamera->mSceneCamera );
	RenderGameObjects( mShaderProgram, mSceneFramebuffer, App->mCamera->mSceneCamera );
	RenderGameObjects( mShaderProgram, mGameFramebuffer, App->mCamera->mGameCamera );

	if( App->mEditor->GetSelectedGO() != -1 )
		RenderGizmo();

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
	int r = (++mNextGOId & 0x000000FF) >>  0;
	int g = (mNextGOId & 0x0000FF00) >>  8;
	int b = (mNextGOId & 0x00FF0000) >> 16;

	mGameObjects.push_back( CGameObject( mNextGOId, name ) );
	mGameObjects.back().PushComponent<CComponentRenderer>( CComponentRenderer( aTextPath, glm::vec3( r, g, b ) ) );

	App->mEditor->SetSelectedGO( mGameObjects.size()-1 );

	mGizmo.SetPosition( static_cast<CComponentTransform*>(mGameObjects[App->mEditor->GetSelectedGO()].GetComponent<TRANSFORM>())->mPosition );

	return true;
}

void CModuleRenderer::RenderGameObjects( const int& aShaderProgram, const int& aFramebuffer, CCamera* aCamera ) const
{
	glBindFramebuffer( GL_FRAMEBUFFER, aFramebuffer );
	glUseProgram( aShaderProgram );

	glUniformMatrix4fv( glGetUniformLocation( aShaderProgram, "view" ), 1, GL_FALSE, &aCamera->GetViewMatrix()[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( aShaderProgram, "projection" ), 1, GL_FALSE, &aCamera->GetProjectionMatrix()[0][0] );

	for( const auto& gameObject : mGameObjects )
	{
		const auto renderer = static_cast<CComponentRenderer*>(gameObject.GetComponent<RENDERER>());
		if( aShaderProgram == mBackShaderProgram )
			glUniform3f( glGetUniformLocation( aShaderProgram, "color" ), renderer->GetBackColor().x/255.0f,
					renderer->GetBackColor().y/255.0f, renderer->GetBackColor().z/255.0f );

		const auto transform = static_cast<CComponentTransform*>(gameObject.GetComponent<TRANSFORM>());
		if( renderer && renderer->HasTexture() )
		{

			glUniformMatrix4fv( glGetUniformLocation( aShaderProgram,"model" ), 1, GL_FALSE, &transform->mModelMatrix[0][0] );

			renderer->RenderTexture();
		}

		const auto animation = static_cast<CComponentAnimation*>(gameObject.GetComponent<ANIMATION>());
		if( animation && animation->HasAnimationStates() )
		{
			auto& animationState = animation->GetCurrentAnimationState();
			if( animationState.HasSprites() && animationState.GetCurrentSprite().HasTexture() )
			{
				animationState.AddTime( App->mTimer->GetDeltaTime() );
				glUniformMatrix4fv( glGetUniformLocation( aShaderProgram,"model" ), 1, GL_FALSE, &transform->mModelMatrix[0][0] );

				animationState.RenderCurrentSprite();
			}
		}
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
		const auto renderer = static_cast<CComponentRenderer*>(mGameObjects[i].GetComponent<RENDERER>());
		if( !renderer || !renderer->HasTexture() )
			continue;
		const glm::vec3& GOColor = renderer->GetBackColor();
		if( GOColor.x == int(color.x*256) &&  GOColor.y == int(color.y*256) && GOColor.z == int(color.z*256) )
		{
			App->mEditor->SetSelectedGO(i);
			mGizmo.SetPosition( static_cast<CComponentTransform*>(mGameObjects[App->mEditor->GetSelectedGO()].GetComponent<TRANSFORM>())->mPosition );
		}
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void CModuleRenderer::RenderColliders()
{
	for( const auto& GO : mGameObjects )
	{
		const auto boxCollider = static_cast<CComponentBoxCollider*>(GO.GetComponent<BOX_COLLIDER>());
		if( boxCollider )
			RenderQuad( boxCollider->GetCenter(), boxCollider->GetSize() );
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
	glUniformMatrix4fv( glGetUniformLocation( mGridShaderProgram, "view" ), 1, GL_FALSE, &App->mCamera->mSceneCamera->GetViewMatrix()[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mGridShaderProgram, "projection" ), 1, GL_FALSE, &App->mCamera->mSceneCamera->GetProjectionMatrix()[0][0] );
	glUniform3f( glGetUniformLocation( mGridShaderProgram, "color" ), 0.0f, 1.0f, 0.0f );

	glBindVertexArray( VAO );
	glLineWidth(2.0f);
	glDrawArrays( GL_LINES, 0, 8 );
	glLineWidth(1.0f);
	glBindVertexArray( 0 );

	glUseProgram( 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void CModuleRenderer::RenderGizmo()
{
	glBindFramebuffer( GL_FRAMEBUFFER, mSceneFramebuffer );
	glUseProgram( mShaderProgram );

	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram, "view" ), 1, GL_FALSE, &App->mCamera->mSceneCamera->GetViewMatrix()[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram, "projection" ), 1, GL_FALSE, &App->mCamera->mSceneCamera->GetProjectionMatrix()[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram, "model" ), 1, GL_FALSE,
			&static_cast<CComponentTransform*>(mGizmo.GetXAxis().GetComponent<TRANSFORM>())->mModelMatrix[0][0] );

	static_cast<CComponentRenderer*>(mGizmo.GetXAxis().GetComponent<RENDERER>())->RenderTexture();
	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram, "model" ), 1, GL_FALSE,
			&static_cast<CComponentTransform*>(mGizmo.GetYAxis().GetComponent<TRANSFORM>())->mModelMatrix[0][0] );
	static_cast<CComponentRenderer*>(mGizmo.GetYAxis().GetComponent<RENDERER>())->RenderTexture();

	glUseProgram( 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
