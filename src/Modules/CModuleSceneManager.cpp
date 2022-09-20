#include "Modules/CModuleSceneManager.h"

#include "CComponentTransform.h"

CModuleSceneManager::CModuleSceneManager() :
	mSelectedGOIndex( -1 ),
	mNextGOId( 0 ),
	mGizmo( false )
{
}

bool CModuleSceneManager::Init()
{
	mGizmo.AttachAxisTextures( "../engine_resources/icons/greenArrow.png", "../engine_resources/icons/blueArrow.png" );
	mGizmo.SetSize( glm::vec2( 1.5f, 1.5f ) );

	return true;
}

bool CModuleSceneManager::Update()
{
	return true;
}

const std::vector<CGameObject>& CModuleSceneManager::GetGameObjects() const
{
	return mGameObjects;
}

void CModuleSceneManager::CreateGameObject()
{
	std::string name = "Game Object"+std::to_string(mNextGOId);
	mGameObjects.push_back( CGameObject( mNextGOId, name ) );
	mNextGOId++;
	SetSelectedGO( mGameObjects.size()-1 );
}

void CModuleSceneManager::AddGameObject( const CGameObject& aGameObject )
{
	mGameObjects.push_back( aGameObject );
	SetSelectedGO( mGameObjects.size()-1 );
}

const CGizmo& CModuleSceneManager::GetGizmo() const
{
	return mGizmo;
}

void CModuleSceneManager::UpdateGizmoPosition()
{
	mGizmo.SetPosition( static_cast<CComponentTransform*>(GetSelectedGO().GetComponent<TRANSFORM>())->GetPosition() );
}

CGameObject& CModuleSceneManager::GetSelectedGO()
{
	return mGameObjects[mSelectedGOIndex];
}

void CModuleSceneManager::SetSelectedGO( const int aIndex )
{
	mSelectedGOIndex = aIndex;
	UpdateGizmoPosition();
}

const bool CModuleSceneManager::HasSelectedGO() const
{
	return mSelectedGOIndex >= 0 && mSelectedGOIndex < mGameObjects.size();
}

void CModuleSceneManager::ClearGameObjects()
{
	mGameObjects.clear();
	mSelectedGOIndex = -1;
}
