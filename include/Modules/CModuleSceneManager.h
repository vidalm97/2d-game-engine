#ifndef CMODULE_SCENE_MANAGER__H
#define CMODULE_SCENE_MANAGER__H

#include "AModule.h"

#include "CGameObject.h"
#include "CGizmo.h"

class CModuleSceneManager : public AModule
{
public:
	CModuleSceneManager();
	~CModuleSceneManager() = default;

	bool Init() override;
	bool Update() override;

	const std::vector<CGameObject>& GetGameObjects() const;
	void CreateGameObject();
	void AddGameObject( const CGameObject& aGameObjects );

	void UpdateGizmoPosition();
	const CGizmo& GetGizmo() const;

	CGameObject& GetSelectedGO();
	void SetSelectedGO( const int aIndex );
	const bool HasSelectedGO() const;

	void ClearGameObjects();

private:
	int mSelectedGOIndex;
	unsigned int mNextGOId;

	CGizmo mGizmo;
	std::vector<CGameObject> mGameObjects;

	friend class CModuleEditor;
};

#endif
