#ifndef CGAMEOBJECT__H
#define CGAMEOBJECT__H

#include "CTexture.h"

#include "glm.hpp"

#include <string>

class CComponentBoxCollider;
class CComponentRenderer;
class CComponentTransform;

class CGameObject
{
public:
	CGameObject() = default;
	CGameObject( const unsigned int aID, CComponentRenderer* aComponentRenderer, const std::string& aName );
	~CGameObject() = default;

	void Clear();

	void CreateComponentBoxCollider();

	unsigned int mID;
	std::string mName;

	CComponentBoxCollider* mComponentBoxCollider = nullptr;
	CComponentRenderer* mComponentRenderer = nullptr;
	CComponentTransform* mComponentTransform = nullptr;
};

#endif
