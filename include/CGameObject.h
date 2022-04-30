#ifndef CGAMEOBJECT__H
#define CGAMEOBJECT__H

#include "CTexture.h"

#include "glm.hpp"

#include <string>

class CGameObject
{
public:
	CGameObject() = default;
	CGameObject( CTexture* aTexture, const std::string& aName );
	~CGameObject() = default;

	void UpdateModelMatrix();
	void Clear();

	glm::mat4 mModelMatrix;
	CTexture* mTexture = nullptr;
	std::string mName;
	glm::vec2 mPosition;
};

#endif
