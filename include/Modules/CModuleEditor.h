#ifndef CMODULEEDITOR__H
#define CMODULEEDITOR__H

#include "AModule.h"

#include "glm.hpp"

class CGameObject;

class CModuleEditor : public AModule
{
public:
	CModuleEditor() = default;
	~CModuleEditor() = default;

	bool Init() override;
	bool Update() override;
	bool PreUpdate() override;
	bool Clear() override;

private:
	void SetUpDockingSpace();
	void RenderHierarchyPanel();
	void RenderGameObjectPanel();
	void RenderScenePanel();
	void RenderGameCameraPanel();

	int mSelectedGO = -1;
	bool mResizedScenePanel = false;
	bool mResizedGamePanel = false;
	bool mShowScene = true;
	bool mShowGameCamera = true;
	glm::vec2 mScenePanelSize = glm::vec2( 0, 0 );
	glm::vec2 mGamePanelSize = glm::vec2( 0, 0 );
};

#endif
