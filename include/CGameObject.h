#ifndef CGAMEOBJECT__H
#define CGAMEOBJECT__H

#include "CSerializator.h"
#include "CTexture.h"

#include "glm.hpp"

#include <string>

class CComponentBoxCollider;
class CComponentRenderer;
class CComponentTransform;

class CGameObject
{
public:
	CGameObject( const unsigned int aID, const std::string& aName );
	CGameObject( const unsigned int aID, CComponentRenderer* aComponentRenderer, const std::string& aName );
	~CGameObject() = default;

	void Clear();

	void CreateComponentBoxCollider();

	void Serialize( CSerializator::json& aJson ) const;
	void Deserialize( const CSerializator::json& aJson );

	unsigned int mID;
	std::string mName;

	CComponentBoxCollider* mComponentBoxCollider = nullptr;
	CComponentRenderer* mComponentRenderer = nullptr;
	CComponentTransform* mComponentTransform = nullptr;
};

#endif
