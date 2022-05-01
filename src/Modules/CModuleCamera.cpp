#include "Modules/CModuleCamera.h"

#include "CApplication.h"
#include "Modules/CModuleRenderer.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"

#include <vector>
#include<iostream>

CModuleCamera::CModuleCamera( const float aCameraSpeed ) : mCameraSpeed(aCameraSpeed)
{
	mModelMatrix = glm::mat4(1.0f);
	mViewMatrix = glm::mat4(1.0f);
	mProjectionMatrix = glm::mat4(1.0f);

	mCameraPos = glm::vec3( 0.0f, 0.0f, 3.0f );
	mCameraFront = glm::vec3( 0.0f, 0.0f, -1.0f );
	mCameraUp = glm::vec3( 0.0f, 1.0f, 0.0f );
}

bool CModuleCamera::Init()
{
	UpdateProjectionMatrix();
	mViewMatrix = glm::lookAt( mCameraPos, mCameraPos+mCameraFront, mCameraUp );

	return true;
}

bool CModuleCamera::Update()
{
	std::vector<float> vertices;

	for( float i = -20.0f; i < 20.0f; i+=0.2f )
	{
		vertices.push_back( -i );
		vertices.push_back( -20.0f );
		vertices.push_back( 0.0f );
		vertices.push_back( -i );
		vertices.push_back( 20.0f );
		vertices.push_back( 0.0f );

		vertices.push_back( i );
		vertices.push_back( -20.0f );
		vertices.push_back( 0.0f );
		vertices.push_back( i );
		vertices.push_back( 20.0f );
		vertices.push_back( 0.0f );

		vertices.push_back( -20.0f );
		vertices.push_back( -i );
		vertices.push_back( 0.0f );
		vertices.push_back( 20.0f );
		vertices.push_back( -i );
		vertices.push_back( 0.0f );

		vertices.push_back( -20.0f );
		vertices.push_back( i );
		vertices.push_back( 0.0f );
		vertices.push_back( 20.0f );
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

	glUseProgram( App->mRenderer->mGridShaderProgram );
	glUniformMatrix4fv( glGetUniformLocation( App->mRenderer->mGridShaderProgram, "view" ), 1, GL_FALSE, &mViewMatrix[0][0] );
	glUniformMatrix4fv( glGetUniformLocation( App->mRenderer->mGridShaderProgram, "projection" ), 1, GL_FALSE, &mProjectionMatrix[0][0] );

	glBindVertexArray( VAO );
	glDrawArrays( GL_LINES, 0, vertices.size()/3 );
	glBindVertexArray( 0 );

	glUseProgram( 0 );

	return true;
}

bool CModuleCamera::Clear()
{
	return true;
}

void CModuleCamera::UpdateProjectionMatrix()
{
	const float ratio = App->mWindow->GetWidth()/App->mWindow->GetHeight();
	mProjectionMatrix = glm::ortho( -1.0f*ratio, +1.0f*ratio, -1.0f, +1.0f, 0.1f, 100.0f );
}

void CModuleCamera::MoveCamera( const glm::vec3& aDirection )
{
	mCameraPos += mCameraSpeed*aDirection;
	mViewMatrix = glm::lookAt( mCameraPos, mCameraPos+mCameraFront, mCameraUp );
}
