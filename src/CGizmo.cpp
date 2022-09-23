#include "CGizmo.h"

#include "CComponentRenderer.h"
#include "CComponentTransform.h"

CGizmo::CGizmo( const bool aIsActive, const unsigned int aXUniqueColorId, const unsigned int aYUniqueColorId ) :
	mIsActive( aIsActive ),
	mXAxis( 0, "XAxisGizmo" ),
	mYAxis( 0, "YAxisGizmo" )
{
	mXAxis.PushComponent<CComponentRenderer>( CComponentRenderer(aXUniqueColorId) );
	mYAxis.PushComponent<CComponentRenderer>( CComponentRenderer(aYUniqueColorId) );
	mXOffset =  glm::vec2( 0.5f, 0.0f );
	mYOffset =  glm::vec2( 0.0f, 0.5f );
}

void CGizmo::AttachAxisTextures( const std::string& aXAxisTextPath, const std::string& aYAxisTextPath )
{
	static_cast<CComponentRenderer*>(mXAxis.GetComponent<RENDERER>())->AttachTexture( aXAxisTextPath );
	static_cast<CComponentRenderer*>(mYAxis.GetComponent<RENDERER>())->AttachTexture( aYAxisTextPath );
	mXOffset *= static_cast<CComponentRenderer*>(mXAxis.GetComponent<RENDERER>())->GetTextureScaleDeviation();
	mYOffset *= static_cast<CComponentRenderer*>(mXAxis.GetComponent<RENDERER>())->GetTextureScaleDeviation();
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
	static_cast<CComponentTransform*>(mXAxis.GetComponent<TRANSFORM>())->SetPosition( aPosition + mXOffset );
	static_cast<CComponentTransform*>(mXAxis.GetComponent<TRANSFORM>())->UpdateModelMatrix();
	static_cast<CComponentTransform*>(mYAxis.GetComponent<TRANSFORM>())->SetPosition( aPosition + mYOffset );
	static_cast<CComponentTransform*>(mYAxis.GetComponent<TRANSFORM>())->UpdateModelMatrix();
}

void CGizmo::SetSize( const glm::vec2& aSize )
{
	static_cast<CComponentTransform*>(mXAxis.GetComponent<TRANSFORM>())->SetScale( aSize );
	static_cast<CComponentTransform*>(mXAxis.GetComponent<TRANSFORM>())->UpdateModelMatrix();
	static_cast<CComponentTransform*>(mYAxis.GetComponent<TRANSFORM>())->SetScale( aSize );
	static_cast<CComponentTransform*>(mYAxis.GetComponent<TRANSFORM>())->UpdateModelMatrix();
	mXOffset *= aSize;
	mYOffset *= aSize;
}

const CGameObject& CGizmo::GetXAxis() const
{
	return mXAxis;
}

const CGameObject& CGizmo::GetYAxis() const
{
	return mYAxis;
}

const glm::vec2& CGizmo::GetXOffset() const
{
	return mXOffset;
}

const glm::vec2& CGizmo::GetYOffset() const
{
	return mYOffset;
}
