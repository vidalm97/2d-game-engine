#include "CCamera.h"

#include "CApplication.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"

CCamera::CCamera( const float aMovementSpeed, const float aFocusSpeed ) : mMovementSpeed(aMovementSpeed), mFocusSpeed(aFocusSpeed)
{
	mModelMatrix = glm::mat4(1.0f);
	mViewMatrix = glm::mat4(1.0f);
	mProjectionMatrix = glm::mat4(1.0f);

	mCameraPos = glm::vec3( 1.0f, 0.0f, 10.0f );
	mCameraFront = glm::vec3( 0.0f, 0.0f, -1.0f );
	mCameraUp = glm::vec3( 0.0f, 1.0f, 0.0f );

	mDistance = 3.0f;

	UpdateProjectionMatrix();
	mViewMatrix = glm::lookAt( mCameraPos, mCameraPos+mCameraFront, mCameraUp );
}

void CCamera::Focus( const float aDistance )
{
	if( mDistance+mFocusSpeed*aDistance > 0.0f )
		mDistance += mFocusSpeed*aDistance;
	UpdateProjectionMatrix();
}

void CCamera::UpdateProjectionMatrix()
{
	const float ratio = App->mWindow->GetWidth()/App->mWindow->GetHeight();
	mProjectionMatrix = glm::ortho( -mDistance*ratio, mDistance*ratio, -mDistance, mDistance, 0.1f, 100.0f );
}

void CCamera::MoveCamera( const glm::vec3& aDirection )
{
	mCameraPos += mMovementSpeed*aDirection;
	mViewMatrix = glm::lookAt( mCameraPos, mCameraPos+mCameraFront, mCameraUp );
}
