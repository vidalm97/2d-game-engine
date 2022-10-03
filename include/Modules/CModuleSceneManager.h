#ifndef CMODULE_SCENE_MANAGER__H
#define CMODULE_SCENE_MANAGER__H

#include "AGizmo.h"
#include "AModule.h"

#include "CComponentRenderer.h"
#include "CGameObject.h"

enum GIZMO_MODE
{
	FREE = -1,
	TRANSLATION = 0,
	SCALE = 1,
	ROTATION = 2
};

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
	const std::unique_ptr<AGizmo>& GetGizmo() const;
	void SetGizmoMode( const GIZMO_MODE& aGizmoMode );
	const GIZMO_MODE& GetGizmoMode() const;

	CGameObject& GetSelectedGO();
	void SetSelectedGO( const int aIndex );
	const bool HasSelectedGO() const;
	bool CheckSelection( int x, int y );
	const bool& IsDragging() const;
	void SetDragging( const bool& aIsDragging );
	void Drag( const glm::vec2& aCurrentDragPos );

	void ClearGameObjects();

private:
	const bool IsRendererSelected( const CComponentRenderer* aRenderer, const glm::vec3& aColor ) const;
	const glm::vec2 GetWorldPosition( const glm::vec2& aScreenCoord ) const;

	int mSelectedGOIndex;
	unsigned int mNextGOId;
	bool mIsDragging;
	glm::vec2 mPreviousDragPos;

	std::vector<std::unique_ptr<AGizmo>> mGizmos;
	GIZMO_MODE mGizmoMode;

	std::vector<CGameObject> mGameObjects;

	friend class CModuleEditor;
};

#endif
