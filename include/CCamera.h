#ifndef CCAMERA__H
#define CCAMERA__H

#include "glm.hpp"

class CCamera
{
public:
	CCamera( const float aMovementSpeed, const float aFocusSpeed );
	~CCamera() = default;

	void MoveCamera( const glm::vec3& aDirection );
	void Focus( const float aDistance );
	void UpdateProjectionMatrix();

	glm::mat4 mModelMatrix;
	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;

private:
	glm::vec3 mCameraPos;
	glm::vec3 mCameraFront;
	glm::vec3 mCameraUp;

	float mDistance;
	float mMovementSpeed;
	float mFocusSpeed;
};

#endif
