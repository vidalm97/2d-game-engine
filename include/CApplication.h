#ifndef CAPPLICATION__H
#define CAPPLICATION__H

#include "Globals.h"

#include <vector>

class AModule;
class CModuleInput;
class CModuleRenderer;
class CModuleWindow;

class CApplication
{
public:
	CApplication() = default;
	~CApplication() = default;

	bool Start();

	EAppStatus Init() const;
	EAppStatus Update() const;
	EAppStatus Clear() const;

	CModuleInput* mInput = nullptr;
	CModuleRenderer* mRenderer = nullptr;
	CModuleWindow* mWindow = nullptr;

private:
	std::vector<AModule*> mModules;
};

extern CApplication* App;

#endif
