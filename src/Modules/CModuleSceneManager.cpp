#include "Modules/CModuleSceneManager.h"

#include "CApplication.h"
#include "CComponentTransform.h"

#include "CRotationGizmo.h"
#include "CScaleGizmo.h"
#include "CTranslationGizmo.h"

#include "Modules/CModuleCamera.h"
#include "Modules/CModuleRenderer.h"
#include "Modules/CModuleWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

CModuleSceneManager::CModuleSceneManager() :
	mSelectedGOIndex( -1 ),
	mNextGOId( 6 ),
	mIsDragging( false ),
	mPreviousDragPos( 0.0f ),
	mGizmoMode(TRANSLATION)
{
}

bool CModuleSceneManager::Init()
{
	SGizmoTexture gizmoTranslateX = { "../engine_resources/icons/Gizmo/translationX.png", 1, {0.5f,0.0f} };
	SGizmoTexture gizmoTranslateY = { "../engine_resources/icons/Gizmo/translationY.png", 2, {0.0f,0.5f} };
	mGizmos.push_back( std::make_unique<CTranslationGizmo>( std::vector<SGizmoTexture>({ gizmoTranslateX, gizmoTranslateY }), glm::vec2( 1.5f, 1.5f ) ) );

	SGizmoTexture gizmoScaleX = { "../engine_resources/icons/Gizmo/scaleX.png", 3, {0.5f,0.0f} };
	SGizmoTexture gizmoScaleY = { "../engine_resources/icons/Gizmo/scaleY.png", 4, {0.0f,0.5f} };
	mGizmos.push_back( std::make_unique<CScaleGizmo>( std::vector<SGizmoTexture>({ gizmoScaleX, gizmoScaleY }), glm::vec2( 1.9f, 1.9f ) ) );

	SGizmoTexture gizmoRotation = { "../engine_resources/icons/Gizmo/rotation.png", 5, {0.0f,0.0f} };
	mGizmos.push_back( std::make_unique<CRotationGizmo>( std::vector<SGizmoTexture>({ gizmoRotation }), glm::vec2( 0.5f, 0.5f ) ) );
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

const std::unique_ptr<AGizmo>& CModuleSceneManager::GetGizmo() const
{
	return mGizmos[mGizmoMode];
}

void CModuleSceneManager::SetGizmoMode( const GIZMO_MODE& aGizmoMode )
{
	mGizmoMode = aGizmoMode;
}

const GIZMO_MODE& CModuleSceneManager::GetGizmoMode() const
{
	return mGizmoMode;
}

void CModuleSceneManager::UpdateGizmoPosition()
{
	for( const auto& gizmo : mGizmos )
		gizmo->SetPosition( static_cast<CComponentTransform*>(GetSelectedGO().GetComponent<TRANSFORM>())->GetPosition() );
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

	if( mGizmoMode != FREE && HasSelectedGO() )
	{
		auto idx = 0;
		for( const auto& gizmoSelectable : mGizmos[mGizmoMode]->GetSelectables() )
		{
			if( IsRendererSelected( static_cast<CComponentRenderer*>(gizmoSelectable.first.GetComponent<RENDERER>()), color ) )
			{
				mIsDragging = true;
				mGizmos[mGizmoMode]->SetCurrentSelectable( idx );
				break;
			}
			idx++;
		}
	}

	for( int i = 0; i < GetGameObjects().size() && !mIsDragging; ++i )
	{
		if( IsRendererSelected( static_cast<CComponentRenderer*>(GetGameObjects()[i].GetComponent<RENDERER>()), color ) )
		{
			mIsDragging = mGizmoMode == FREE;
			SetSelectedGO(i);
			break;
		}
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	if( mIsDragging )
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

const bool& CModuleSceneManager::IsDragging() const
{
	return mIsDragging;
}

void CModuleSceneManager::SetDragging( const bool& aIsDragging )
{
	mIsDragging = aIsDragging;
}

void CModuleSceneManager::Drag( const glm::vec2& aCurrentDragPos )
{
	auto currentDragPos = GetWorldPosition( aCurrentDragPos );
	auto deltaPos = currentDragPos-mPreviousDragPos;

	auto* transform = static_cast<CComponentTransform*>(GetSelectedGO().GetComponent<TRANSFORM>());
	if( mGizmoMode != FREE )
		mGizmos[mGizmoMode]->UpdateTransform( transform, mPreviousDragPos, currentDragPos );
	else
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
