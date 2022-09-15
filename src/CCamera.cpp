#include "CCamera.h"

#include "CApplication.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"

CCamera::CCamera( const float aMovementSpeed, const float aFocusSpeed, const float aDistance, const glm::vec3& aPosition,
		const glm::vec3& aFront, const glm::vec3& aUp ) :
	mMovementSpeed( aMovementSpeed ),
	mFocusSpeed( aFocusSpeed ),
	mDistance( aDistance ),
	mPosition( aPosition ),
	mFront( aFront ),
	mUp( aUp )
{
	UpdateProjectionMatrix();
	mViewMatrix = glm::lookAt( mPosition, mPosition+mFront, mUp );
}

const glm::mat4& CCamera::GetViewMatrix() const
{
	return mViewMatrix;
}

const glm::mat4& CCamera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
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
	mPosition += mMovementSpeed*aDirection;
	mViewMatrix = glm::lookAt( mPosition, mPosition+mFront, mUp );
}
