#include "CGameObject.h"

#include "CComponentBoxCollider.h"
#include "CComponentRenderer.h"
#include "CComponentTransform.h"

CGameObject::CGameObject( const unsigned int aID, CComponentRenderer* aComponentRenderer, const std::string& aName ) :
	mID( aID ),
	mComponentRenderer( aComponentRenderer ),
	mName( aName )
{
	mComponentTransform = new CComponentTransform();
}

void CGameObject::Clear()
{
	delete mComponentBoxCollider;
	delete mComponentTransform;
	delete mComponentRenderer;
}

void CGameObject::CreateComponentBoxCollider()
{
	if( mComponentBoxCollider )
		return;

	mComponentBoxCollider = new CComponentBoxCollider( mComponentTransform->mPosition, mComponentRenderer && mComponentRenderer->HasTexture() ?
			glm::vec2( mComponentTransform->mScale.x*mComponentRenderer->GetTextureWidth()/mComponentRenderer->GetTextureHeight(), mComponentTransform->mScale.y ) : glm::vec2( 10, 10 ),  false );
}
