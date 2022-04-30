#include "CGameObject.h"

#include "gtc/matrix_transform.hpp"

CGameObject::CGameObject( CTexture* aTexture, const std::string& aName ) :
	mTexture( aTexture ),
	mName( aName )
{
	mModelMatrix = glm::mat4(1.0f);
	mPosition = glm::vec2( 0.0f );
}

void CGameObject::UpdateModelMatrix()
{
	mModelMatrix[3][0] = mPosition.x;
	mModelMatrix[3][1] = mPosition.y;
}

void CGameObject::Clear()
{
	delete mTexture;
}
