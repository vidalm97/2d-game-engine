#ifndef CMODULECAMERA__H
#define CMODULECAMERA__H

#include "AModule.h"

#include "CCamera.h"

#include "glm.hpp"

class CModuleCamera : public AModule
{
public:
	CModuleCamera();
	~CModuleCamera() = default;

	bool Init() override;
	bool Update() override;
	bool Clear() override;

	CCamera* mSceneCamera = nullptr;
	CCamera* mGameCamera = nullptr;
};

#endif
