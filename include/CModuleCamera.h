#ifndef CMODULECAMERA__H
#define CMODULECAMERA__H

#include "AModule.h"

class CModuleCamera : public AModule
{
public:
	CModuleCamera() = default;
	~CModuleCamera() = default;

	bool Init() override;
	bool Update() override;
	bool Clear() override;
};

#endif
