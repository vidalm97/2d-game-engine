#ifndef CMODULE_TIME__H
#define CMODULE_TIME__H

#include "AModule.h"

class CModuleTime : public AModule
{
public:
	CModuleTime();
	~CModuleTime() = default;

	bool Update() override;

	float GetDeltaTime() const;

private:
	float mLastTime;
	float mDeltaTime;
};

#endif
