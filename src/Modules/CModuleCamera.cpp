#include "Modules/CModuleCamera.h"

#include "CApplication.h"
#include "CCamera.h"
#include "Modules/CModuleRenderer.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"

#include <vector>

CModuleCamera::CModuleCamera()
{
	mSceneCamera = new CCamera( 0.01f, 0.02f );
	mGameCamera = new CCamera( 0.01f, 0.02f );
}

bool CModuleCamera::Init()
{
	return true;
}

bool CModuleCamera::Update()
{
	std::vector<float> vertices;

	for( float i = -200.0f; i <= 200.0f; i+=0.25f )
	{
		vertices.push_back( i );
		vertices.push_back( -200.0f );
		vertices.push_back( 0.0f );
		vertices.push_back( i );
		vertices.push_back( 200.0f );
		vertices.push_back( 0.0f );

		vertices.push_back( -200.0f );
		vertices.push_back( i );
		vertices.push_back( 0.0f );
		vertices.push_back( 200.0f );
		vertices.push_back( i );
		vertices.push_back( 0.0f );
	}

	unsigned int VBO;
	glGenBuffers( 1, &VBO );
	unsigned int VAO;
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 );
	glEnableVertexAttribArray( 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, App->mRenderer->mSceneFramebuffer );

	glUseProgram( App->mRenderer->mGridShaderProgram );
	glUniformMatrix4fv( glGetUniformLocation( App->mRenderer->mGridShaderProgram, "view" ), 1, GL_FALSE, &mSceneCamera->mViewMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( App->mRenderer->mGridShaderProgram, "projection" ), 1, GL_FALSE, &mSceneCamera->mProjectionMatrix[0][0] );

	glBindVertexArray( VAO );
	glDrawArrays( GL_LINES, 0, vertices.size()/3 );
	glBindVertexArray( 0 );

	glUseProgram( 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	return true;
}

bool CModuleCamera::Clear()
{
	delete mSceneCamera;
	delete mGameCamera;

	return true;
}

