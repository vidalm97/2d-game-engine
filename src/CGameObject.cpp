#include "CGameObject.h"

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
	delete mComponentTransform;
	delete mComponentRenderer;
}
