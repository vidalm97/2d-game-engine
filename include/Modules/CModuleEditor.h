#ifndef CMODULEEDITOR__H
#define CMODULEEDITOR__H

#include "AModule.h"

#include "glm.hpp"

#include <filesystem>

class CGameObject;
class CTexture;

class CModuleEditor : public AModule
{
public:
	CModuleEditor();
	~CModuleEditor() = default;

	bool Init() override;
	bool Update() override;
	bool PreUpdate() override;
	bool Clear() override;

private:
	void SetUpDockingSpace();
	void RenderSerializationPanel();
	void RenderHierarchyPanel();
	void RenderGameObjectPanel();
	void RenderScenePanel();
	void RenderGameCameraPanel();
	void RenderGameControlPanel();
	void RenderResourcePanel();
	void RenderAddComponentPanel();
	void RenderRendererPanel();
	void RenderBoxColliderPanel();
	void RenderAnimationPanel();

	bool mResizedScenePanel = false;
	bool mResizedGamePanel = false;
	bool mShowScene = true;
	bool mShowGameCamera = true;
	glm::vec2 mScenePanelSize = glm::vec2( 0, 0 );
	glm::vec2 mGamePanelSize = glm::vec2( 0, 0 );

	CTexture* mPlayIcon = nullptr;
	CTexture* mPauseIcon = nullptr;
	CTexture* mStopIcon = nullptr;

	CTexture* mDirectoryIcon = nullptr;
	CTexture* mFileIcon = nullptr;

	std::filesystem::path mCurrentDirectory;
};

#endif
