#ifndef CGAMEOBJECT__H
#define CGAMEOBJECT__H

#include "AComponent.h"
#include "CSerializator.h"
#include "CTexture.h"

#include "glm.hpp"

#include <memory>
#include <string>

class CGameObject
{
public:
	CGameObject( const unsigned int aID, const std::string& aName );
	~CGameObject() = default;
	CGameObject( const CGameObject& );

	const std::string& GetName() const;
	void SetName( const std::string& aName );
	const unsigned int GetID() const;

	template <typename T> void PushComponent( const T& aComponent );
	template <COMPONENT_TYPE type> AComponent* GetComponent() const;

	void Serialize( CSerializator::json& aJson ) const;
	void Deserialize( const CSerializator::json& aJson );

private:
	unsigned int mID;
	std::string mName;

	std::vector<std::unique_ptr<AComponent>> mComponents;
};

template <typename T> void CGameObject::PushComponent( const T& aComponent )
{
	mComponents.push_back( std::make_unique<T>(aComponent) );
}

template <COMPONENT_TYPE type> AComponent* CGameObject::GetComponent() const
{
	for( auto& component : mComponents )
		if( component->GetType() == type )
			return component.get();

	return nullptr;
}

#endif
