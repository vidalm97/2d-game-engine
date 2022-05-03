#include "CGameObject.h"

#include "CComponentRenderer.h"
#include "CComponentTransform.h"

CGameObject::CGameObject( CComponentRenderer* aComponentRenderer, const std::string& aName ) :
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
