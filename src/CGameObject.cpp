#include "CGameObject.h"

#include "CComponentTransform.h"

CGameObject::CGameObject( CTexture* aTexture, const std::string& aName ) :
	mTexture( aTexture ),
	mName( aName )
{
	mComponentTransform = new CComponentTransform();
}

void CGameObject::Clear()
{
	delete mComponentTransform;
	delete mTexture;
}
