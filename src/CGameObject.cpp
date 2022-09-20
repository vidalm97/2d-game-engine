#include "CGameObject.h"

#include "CComponentAnimation.h"
#include "CComponentBoxCollider.h"
#include "CComponentRenderer.h"
#include "CComponentTransform.h"

CGameObject::CGameObject( const unsigned int aID, const std::string& aName ) :
	mID( aID ),
	mName( aName )
{
	mComponents.push_back( std::make_unique<CComponentTransform>() );
}

CGameObject::CGameObject( const CGameObject& aGameObject ) :
	mID( aGameObject.mID ),
	mName( aGameObject.mName )
{
	for( const auto& component : aGameObject.mComponents )
		mComponents.push_back( component->Clone() );
}

const std::string& CGameObject::GetName() const
{
	return mName;
}

void CGameObject::SetName( const std::string& aName )
{
	mName = aName;
}

const unsigned int CGameObject::GetID() const
{
	return mID;
}

void CGameObject::Serialize( CSerializator::json& aJson ) const
{
	aJson["ID"] = mID;
	aJson["name"] = mName;

	static_cast<CComponentTransform*>(GetComponent<TRANSFORM>())->Serialize( aJson["Transform"] );

	const auto renderer = static_cast<CComponentRenderer*>(GetComponent<RENDERER>());
	if( renderer )
		renderer->Serialize( aJson["Renderer"] );
}

void CGameObject::Deserialize( const CSerializator::json& aJson )
{
	aJson.at("ID").get_to(mID);
	aJson.at("name").get_to(mName);

	static_cast<CComponentTransform*>(GetComponent<TRANSFORM>())->Deserialize( aJson.at("Transform") );

	if( aJson.contains("Renderer") )
	{
		PushComponent<CComponentRenderer>( CComponentRenderer(mID) );
		static_cast<CComponentRenderer*>(GetComponent<RENDERER>())->Deserialize( aJson.at("Renderer") );
	}
}
