#ifndef CAPPLICATION__H
#define CAPPLICATION__H

#include "Globals.h"

#include <vector>

class AModule;
class CModuleCamera;
class CModuleEditor;
class CModuleInput;
class CModuleRenderer;
class CModuleResourceManager;
class CModuleSceneManager;
class CModuleTime;
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

	CModuleCamera* mCamera = nullptr;
	CModuleEditor* mEditor = nullptr;
	CModuleInput* mInput = nullptr;
	CModuleRenderer* mRenderer = nullptr;
	CModuleResourceManager* mResourceManager = nullptr;
	CModuleSceneManager* mSceneManager = nullptr;
	CModuleTime* mTimer = nullptr;
	CModuleWindow* mWindow = nullptr;

private:
	std::vector<AModule*> mModules;
};

extern CApplication* App;

#endif
