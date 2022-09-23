#ifndef CMODULE_SCENE_MANAGER__H
#define CMODULE_SCENE_MANAGER__H

#include "AModule.h"

#include "CComponentRenderer.h"
#include "CGameObject.h"
#include "CGizmo.h"

enum E_DRAG_SELECTION_STATE
{
	NONE,
	GAME_OBJECT,
	X_GIZMO,
	Y_GIZMO
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
	const CGizmo& GetGizmo() const;

	CGameObject& GetSelectedGO();
	void SetSelectedGO( const int aIndex );
	const bool HasSelectedGO() const;
	bool CheckSelection( int x, int y );
	const E_DRAG_SELECTION_STATE& GetDragSelectionState() const;
	void SetDragSelectionState( const E_DRAG_SELECTION_STATE& aDragSelectionState );
	void Drag( const glm::vec2& aCurrentDragPos );

	void ClearGameObjects();

private:
	const bool IsRendererSelected( const CComponentRenderer* aRenderer, const glm::vec3& aColor ) const;
	const glm::vec2 GetWorldPosition( const glm::vec2& aScreenCoord ) const;

	int mSelectedGOIndex;
	unsigned int mNextGOId;
	E_DRAG_SELECTION_STATE mDragSelectionState;
	glm::vec2 mPreviousDragPos;

	CGizmo mGizmo;
	std::vector<CGameObject> mGameObjects;

	friend class CModuleEditor;
};

#endif
