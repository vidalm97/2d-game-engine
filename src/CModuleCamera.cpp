#include "CModuleCamera.h"

#include "CApplication.h"
#include "CModuleWindow.h"

#include "gtc/matrix_transform.hpp"

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
	mProjectionMatrix = glm::ortho( -1.0f, +1.0f, -1.0f, +1.0f, 0.1f, 100.0f );
	mViewMatrix = glm::lookAt( mCameraPos, mCameraPos+mCameraFront, mCameraUp );

	return true;
}

bool CModuleCamera::Update()
{
	return true;
}

bool CModuleCamera::Clear()
{
	return true;
}

void CModuleCamera::MoveCamera( const glm::vec3& aDirection )
{
	mCameraPos += mCameraSpeed*aDirection;
	mViewMatrix = glm::lookAt( mCameraPos, mCameraPos+mCameraFront, mCameraUp );
}
