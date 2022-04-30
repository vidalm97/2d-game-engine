#ifndef CMODULEEDITOR__H
#define CMODULEEDITOR__H

#include "AModule.h"

class CGameObject;

class CModuleEditor : public AModule
{
public:
	CModuleEditor() = default;
	~CModuleEditor() = default;

	bool Init() override;
	bool Update() override;
	bool Clear() override;

private:
	void RenderHierarchyPanel();
	void RenderGameObjectPanel();

	int mSelectedGO = -1;
};

#endif
