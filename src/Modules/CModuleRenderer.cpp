#include "Modules/CModuleRenderer.h"

#include "CApplication.h"
#include "CComponentRenderer.h"
#include "CComponentTransform.h"
#include "Modules/CModuleCamera.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

bool CModuleRenderer::Init()
{
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexCoord;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection*view*model*vec4( aPos.x, aPos.y, aPos.z, 1.0 );\n"
		"	TexCoord = vec2( aTexCoord.x, aTexCoord.y );\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D texture1;\n"
		"void main()\n"
		"{\n"
		"	FragColor = texture( texture1, TexCoord );\n"
		"}\0";

	const char* vertexGridShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"out vec3 color;\n"
		"void main()\n"
		"{\n"
			"color = vec3( 0.55, 0.55, 0.55);\n"
			"if( mod(aPos.x,2.5) < 0.01 && mod(aPos.y,2.5) < 0.01 )\n"
			"{\n"
			"	color.x = 0.3f;\n"
			"	color.y = 0.3f;\n"
			"	color.z = 0.3f;\n"
			"}\n"
		"	gl_Position = projection*view*vec4( aPos.x, aPos.y, aPos.z, 1.0 );\n"
		"}\0";

	const char* fragmentGridShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 color;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4( color.x, color.y, color.z, 1.0 );\n"
		"}\0";

	CreateShader( mShaderProgram, vertexShaderSource, fragmentShaderSource );
	CreateShader( mGridShaderProgram, vertexGridShaderSource, fragmentGridShaderSource );

	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"view" ), 1, GL_FALSE, &App->mCamera->mViewMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"projection" ), 1, GL_FALSE, &App->mCamera->mProjectionMatrix[0][0] );

	InitGameFramebuffer();
	return InitSceneFramebuffer();
}

bool CModuleRenderer::PreUpdate()
{
	glBindFramebuffer( GL_FRAMEBUFFER, mSceneFramebuffer );
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, mGameFramebuffer );
	glClearColor( 0.0f, 0.6f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	return true;
}

bool CModuleRenderer::Update()
{
	RenderGameObjects();

	return true;
}

bool CModuleRenderer::Clear()
{
	glDeleteFramebuffers( 1, &mSceneFramebuffer );
	glDeleteTextures( 1, &mSceneFramebufferTexture );
	glDeleteTextures( 1, &mSceneDepthAttachment );

	glDeleteFramebuffers( 1, &mGameFramebuffer );
	glDeleteTextures( 1, &mGameFramebufferTexture );
	glDeleteTextures( 1, &mGameDepthAttachment );

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

void CModuleRenderer::RenderGameObjects() const
{
	glBindFramebuffer( GL_FRAMEBUFFER, mGameFramebuffer );
	glUseProgram( mShaderProgram );

	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram, "view" ), 1, GL_FALSE, &App->mCamera->mViewMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram, "projection" ), 1, GL_FALSE, &App->mCamera->mProjectionMatrix[0][0] );

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

bool CModuleRenderer::InitSceneFramebuffer()
{
	glGenFramebuffers( 1, &mSceneFramebuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, mSceneFramebuffer );

	glGenTextures( 1, &mSceneFramebufferTexture );
	glBindTexture( GL_TEXTURE_2D, mSceneFramebufferTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mSceneWidth, mSceneHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glBindTexture( GL_TEXTURE_2D, 0 );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mSceneFramebufferTexture, 0 );

	if( glCheckFramebufferStatus( GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		return true;
	}

	return false;
}

bool CModuleRenderer::InitGameFramebuffer()
{
	glGenFramebuffers( 1, &mGameFramebuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, mGameFramebuffer );

	glGenTextures( 1, &mGameFramebufferTexture );
	glBindTexture( GL_TEXTURE_2D, mGameFramebufferTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mGameWidth, mGameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glBindTexture( GL_TEXTURE_2D, 0 );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGameFramebufferTexture, 0 );

	if( glCheckFramebufferStatus( GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		return true;
	}

	return false;
}

void CModuleRenderer::ResizeSceneFramebuffer( const int aSceneWidth, const int aSceneHeight )
{
	glDeleteFramebuffers( 1, &mSceneFramebuffer );
	glDeleteTextures( 1, &mSceneFramebufferTexture );
	glDeleteTextures( 1, &mSceneDepthAttachment );

	mSceneWidth = aSceneWidth;
	mSceneHeight = aSceneHeight;

	InitSceneFramebuffer();
}

void CModuleRenderer::ResizeGameFramebuffer( const int aGameWidth, const int aGameHeight )
{
	glDeleteFramebuffers( 1, &mGameFramebuffer );
	glDeleteTextures( 1, &mGameFramebufferTexture );
	glDeleteTextures( 1, &mGameDepthAttachment );

	mGameWidth = aGameWidth;
	mGameHeight = aGameHeight;

	InitGameFramebuffer();
}
