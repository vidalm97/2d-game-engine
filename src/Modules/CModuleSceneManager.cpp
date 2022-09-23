#include "Modules/CModuleSceneManager.h"

#include "CApplication.h"
#include "CComponentTransform.h"
#include "Modules/CModuleCamera.h"
#include "Modules/CModuleRenderer.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

CModuleSceneManager::CModuleSceneManager() :
	mSelectedGOIndex( -1 ),
	mNextGOId( 3 ),
	mGizmo( true, 1, 2 ),
	mDragSelectionState( NONE ),
	mPreviousDragPos( 0.0f )
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
	if( HasSelectedGO() )
		UpdateGizmoPosition();
}

const bool CModuleSceneManager::HasSelectedGO() const
{
	return mSelectedGOIndex >= 0 && mSelectedGOIndex < mGameObjects.size();
}

bool CModuleSceneManager::CheckSelection( int x, int y )
{
	glBindFramebuffer( GL_READ_FRAMEBUFFER, App->mRenderer->mBackFramebuffer );
	glReadBuffer( GL_COLOR_ATTACHMENT0 );

	glm::vec3 color;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);

	bool selected = false;
	if( HasSelectedGO() )
	{
		if( IsRendererSelected( static_cast<CComponentRenderer*>(mGizmo.GetXAxis().GetComponent<RENDERER>()), color ) )
		{
			selected = true;
			mDragSelectionState = X_GIZMO;
		}
		else if( IsRendererSelected( static_cast<CComponentRenderer*>(mGizmo.GetYAxis().GetComponent<RENDERER>()), color ) )
		{
			selected = true;
			mDragSelectionState = Y_GIZMO;
		}
	}

	for( int i = 0; i < GetGameObjects().size() && !selected; ++i )
	{
		if( IsRendererSelected( static_cast<CComponentRenderer*>(GetGameObjects()[i].GetComponent<RENDERER>()), color ) )
		{
			selected = true;
			SetSelectedGO(i);
			mDragSelectionState = GAME_OBJECT;
		}
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	if( selected )
	{
		mPreviousDragPos = GetWorldPosition( glm::vec2( x,y ) );
		return true;
	}

	return false;
}

const bool CModuleSceneManager::IsRendererSelected( const CComponentRenderer* aRenderer, const glm::vec3& aColor ) const
{
	if( !aRenderer || !aRenderer->HasTexture() )
		return false;

	const glm::vec3& GOColor = aRenderer->GetBackColor();
	if( GOColor.x == int(aColor.x*256) &&  GOColor.y == int(aColor.y*256) && GOColor.z == int(aColor.z*256) )
		return true;

	return false;
}

const E_DRAG_SELECTION_STATE& CModuleSceneManager::GetDragSelectionState() const
{
	return mDragSelectionState;
}

void CModuleSceneManager::SetDragSelectionState( const E_DRAG_SELECTION_STATE& aDragSelectionState )
{
	mDragSelectionState = aDragSelectionState;
}

void CModuleSceneManager::Drag( const glm::vec2& aCurrentDragPos )
{
	auto currentDragPos = GetWorldPosition( aCurrentDragPos );
	auto deltaPos = currentDragPos-mPreviousDragPos;

	if( mDragSelectionState == X_GIZMO )
		deltaPos.y = 0;
	else if( mDragSelectionState == Y_GIZMO )
		deltaPos.x = 0;

	auto* transform = static_cast<CComponentTransform*>(GetSelectedGO().GetComponent<TRANSFORM>());
	transform->SetPosition( transform->GetPosition() + ( deltaPos ) );

	mPreviousDragPos = currentDragPos;
	UpdateGizmoPosition();
}

const glm::vec2 CModuleSceneManager::GetWorldPosition( const glm::vec2& aScreenCoord ) const
{
	double clippedX = (2/App->mWindow->GetWidth())*aScreenCoord.x-1;
	double clippedY = (2/App->mWindow->GetHeight())*aScreenCoord.y-1;

	glm::mat4 mat = glm::inverse( App->mCamera->mSceneCamera->GetProjectionMatrix() * App->mCamera->mSceneCamera->GetViewMatrix() );
	auto pos = mat * glm::vec4(clippedX, clippedY, 0, 1);
	return glm::vec2( pos.x, pos.y );
}

void CModuleSceneManager::ClearGameObjects()
{
	mGameObjects.clear();
	mSelectedGOIndex = -1;
}
