#include "CModuleRenderer.h"

#include "CApplication.h"
#include "CModuleCamera.h"
#include "CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

bool CModuleRenderer::Init()
{
	return true;
}

bool CModuleRenderer::Update()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0);\n"
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

	unsigned int VBO;
	glGenBuffers( 1, &VBO );
	unsigned int VAO;
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 );
	glEnableVertexAttribArray( 0 );

	glUseProgram( mShaderProgram );

	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"model" ), 1, GL_FALSE, &App->mCamera->mViewMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"view" ), 1, GL_FALSE, &App->mCamera->mModelMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( mShaderProgram,"projection" ), 1, GL_FALSE, &App->mCamera->mProjectionMatrix[0][0] );

	glBindVertexArray( VAO );
	glDrawArrays( GL_TRIANGLES, 0, 3 );

	return true;
}

bool CModuleRenderer::Clear()
{
	return true;
}
