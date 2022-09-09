#include "Modules/CModuleTime.h"

#include <GLFW/glfw3.h>

CModuleTime::CModuleTime() :
	mLastTime(0.0f),
	mDeltaTime(0.0f)
{
}

bool CModuleTime::Update()
{
	const auto currentTime = glfwGetTime();
	mDeltaTime = currentTime - mLastTime;
	mLastTime = currentTime;

	return true;
}

float CModuleTime::GetDeltaTime() const
{
	return mDeltaTime;
}
