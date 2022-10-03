#include "AGizmo.h"
#include <iostream>

#include "CComponentRenderer.h"

AGizmo::AGizmo( const std::vector<SGizmoTexture>& aGizmoTextures, const glm::vec2& aSize ) :
	mSize( aSize )
{
	AddSelectables( aGizmoTextures );
	SetSize( aSize );
}

void AGizmo::SetPosition( const glm::vec2& aPosition )
{
	for( auto& selectable : mSelectables )
		static_cast<CComponentTransform*>(selectable.first.GetComponent<TRANSFORM>())->SetPosition( aPosition + selectable.second );
}

void AGizmo::SetSize( const glm::vec2& aSize )
{
	for( auto& selectable : mSelectables )
	{
		selectable.second *= aSize/static_cast<CComponentTransform*>(selectable.first.GetComponent<TRANSFORM>())->GetScale();
		static_cast<CComponentTransform*>(selectable.first.GetComponent<TRANSFORM>())->SetScale( aSize );
	}
	mSize = aSize;
}

const glm::vec2& AGizmo::GetSize() const
{
	return mSize;
}

void AGizmo::SetCurrentSelectable( const int aCurrentSelectable )
{
	mCurrentSelectable = aCurrentSelectable;
}

const std::vector<std::pair<CGameObject, glm::vec2>>& AGizmo::GetSelectables() const
{
	return mSelectables;
}

void AGizmo::AddSelectables( const std::vector<SGizmoTexture>& aGizmoTextures )
{
	for( const auto& aGizmoTexture : aGizmoTextures )
	{
		mSelectables.push_back( { CGameObject( 0, "gizmo" ), aGizmoTexture.mOffset } );
		mSelectables.back().first.PushComponent<CComponentRenderer>( CComponentRenderer( aGizmoTexture.mTexturePath, aGizmoTexture.mUniqueColorId ) ); 
		mSelectables.back().second *= static_cast<CComponentRenderer*>(mSelectables.back().first.GetComponent<RENDERER>())->GetTextureScaleDeviation();
	}
}
