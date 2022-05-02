#ifndef CMODULECAMERA__H
#define CMODULECAMERA__H

#include "AModule.h"

#include "glm.hpp"

class CModuleCamera : public AModule
{
public:
	CModuleCamera( const float aMovementSpeed, const float aFocusSpeed );
	~CModuleCamera() = default;

	bool Init() override;
	bool Update() override;
	bool Clear() override;

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
