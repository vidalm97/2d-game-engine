#include "CGameObject.h"

#include "CComponentAnimation.h"
#include "CComponentBoxCollider.h"
#include "CComponentRenderer.h"
#include "CComponentTransform.h"

CGameObject::CGameObject( const unsigned int aID, const std::string& aName ) :
	mID( aID ),
	mName( aName )
{
	mComponentTransform = new CComponentTransform();
}

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

void CGameObject::CreateComponentAnimation()
{
	if( mComponentAnimation )
		return;

	mComponentAnimation = new CComponentAnimation();
}

void CGameObject::Serialize( CSerializator::json& aJson ) const
{
	aJson["ID"] = mID;
	aJson["name"] = mName;

	mComponentTransform->Serialize( aJson["Transform"] );

	if( mComponentRenderer )
		mComponentRenderer->Serialize( aJson["Renderer"] );
}

void CGameObject::Deserialize( const CSerializator::json& aJson )
{
	aJson.at("ID").get_to(mID);
	aJson.at("name").get_to(mName);

	mComponentTransform->Deserialize( aJson.at("Transform") );

	if( aJson.contains("Renderer") )
	{
		mComponentRenderer = new CComponentRenderer();
		mComponentRenderer->Deserialize( aJson.at("Renderer") );
	}
}
