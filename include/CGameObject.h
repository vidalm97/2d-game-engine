#ifndef CGAMEOBJECT__H
#define CGAMEOBJECT__H

#include "CTexture.h"

#include "glm.hpp"

#include <string>

class CComponentTransform;

class CGameObject
{
public:
	CGameObject() = default;
	CGameObject( CTexture* aTexture, const std::string& aName );
	~CGameObject() = default;

	void Clear();

	std::string mName;
	CTexture* mTexture = nullptr;
	CComponentTransform* mComponentTransform = nullptr;
};

#endif
