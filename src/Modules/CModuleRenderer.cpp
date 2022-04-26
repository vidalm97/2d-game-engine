#include "Modules/CModuleRenderer.h"

#include "CApplication.h"
#include "Modules/CModuleCamera.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

bool CModuleRenderer::Init()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 1.0f
	};

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

	unsigned int vertexShader;
	vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
	glCompileShader( vertexShader );

	int success;
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
	if( !success )
		return false;

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D texture1;\n"
		"void main()\n"
		"{\n"
		"	FragColor = texture( texture1, TexCoord );\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
	glCompileShader( fragmentShader );

	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
	if( !success )
		return false;

	mShaderProgram = glCreateProgram();
	glAttachShader( mShaderProgram, vertexShader );
	glAttachShader( mShaderProgram, fragmentShader );
	glLinkProgram( mShaderProgram );

	glGetProgramiv( mShaderProgram, GL_LINK_STATUS, &success );
	if(!success)
		return false;

	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );

	glGenBuffers( 1, &mVBO );
	glGenVertexArrays( 1, &mVAO );
	glBindVertexArray( mVAO );
	glBindBuffer( GL_ARRAY_BUFFER, mVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0 );
	glEnableVertexAttribArray( 0 );

	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)) );
	glEnableVertexAttribArray( 1 );

	return GenerateTexture( "../resources/textures/test.jpg" );
}

bool CModuleRenderer::Update()
{
	RenderTextures();

	return true;
}

bool CModuleRenderer::Clear()
{
	return true;
}

bool CModuleRenderer::GenerateTexture( const std::string& aTextPath )
{
	unsigned int textureId;
	glGenTextures( 1, &textureId );

	glBindTexture( GL_TEXTURE_2D, textureId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	int width, height, nrChannels;
	unsigned char *data = stbi_load( aTextPath.c_str(), &width, &height, &nrChannels, 0 );
	if (data)
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
	else
	{
		return false;
	}
	stbi_image_free( data );

	mTextures.push_back( CTexture( textureId, width, height, nrChannels ) );

	return true;
}

void CModuleRenderer::RenderTextures() const
{
	for( const auto& texture : mTextures )
	{
		glBindTexture( GL_TEXTURE_2D, texture.mTextureId );
		glUseProgram( mShaderProgram );

		glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"model" ), 1, GL_FALSE, &App->mCamera->mViewMatrix[0][0] );
		glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"view" ), 1, GL_FALSE, &App->mCamera->mModelMatrix[0][0] );
		glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"projection" ), 1, GL_FALSE, &App->mCamera->mProjectionMatrix[0][0] );

		glBindVertexArray( mVAO );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
	}
}
