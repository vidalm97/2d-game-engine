#include "CGizmo.h"

#include "CComponentRenderer.h"
#include "CComponentTransform.h"

CGizmo::CGizmo( const bool aIsActive ) :
	mIsActive( aIsActive ),
	mXAxis( 0, new CComponentRenderer(), "XAxisGizmo" ),
	mYAxis( 0, new CComponentRenderer(), "YAxisGizmo" )
{
	mXOffset =  glm::vec2( 0.5f, 0.0f );
	mYOffset =  glm::vec2( 0.0f, 0.5f );
}

void CGizmo::AttachAxisTextures( const std::string& aXAxisTextPath, const std::string& aYAxisTextPath )
{
	mXAxis.mComponentRenderer->AttachTexture( aXAxisTextPath );
	mYAxis.mComponentRenderer->AttachTexture( aYAxisTextPath );
	mXOffset *= mXAxis.mComponentRenderer->GetTextureScaleDeviation();
	mYOffset *= mXAxis.mComponentRenderer->GetTextureScaleDeviation();
}

const bool CGizmo::IsActive() const
{
	return mIsActive;
}

void CGizmo::SetActive( const bool aIsActive )
{
	mIsActive = aIsActive;
}

void CGizmo::SetPosition( const glm::vec2& aPosition )
{
	mXAxis.mComponentTransform->mPosition = aPosition + mXOffset;
	mXAxis.mComponentTransform->UpdateModelMatrix();
	mYAxis.mComponentTransform->mPosition = aPosition + mYOffset;
	mYAxis.mComponentTransform->UpdateModelMatrix();
}

void CGizmo::SetSize( const glm::vec2& aSize )
{
	mXAxis.mComponentTransform->mScale = aSize;
	mXAxis.mComponentTransform->UpdateModelMatrix();
	mYAxis.mComponentTransform->mScale = aSize;
	mYAxis.mComponentTransform->UpdateModelMatrix();
	mXOffset *= aSize;
	mYOffset *= aSize;
}

const glm::vec2& CGizmo::GetXOffset() const
{
	return mXOffset;
}

const glm::vec2& CGizmo::GetYOffset() const
{
	return mYOffset;
}
