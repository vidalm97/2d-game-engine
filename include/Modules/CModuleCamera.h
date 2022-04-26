#ifndef CMODULECAMERA__H
#define CMODULECAMERA__H

#include "AModule.h"

#include "glm.hpp"

class CModuleCamera : public AModule
{
public:
	CModuleCamera( const float aCameraSpeed );
	~CModuleCamera() = default;

	bool Init() override;
	bool Update() override;
	bool Clear() override;

	void MoveCamera( const glm::vec3& aDirection );

	glm::mat4 mModelMatrix;
	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;

private:
	glm::vec3 mCameraPos;
	glm::vec3 mCameraFront;
	glm::vec3 mCameraUp;

	float mCameraSpeed;
};

#endif
