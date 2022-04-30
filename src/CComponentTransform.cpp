#include "CComponentTransform.h"

#include "gtc/matrix_transform.hpp"

CComponentTransform::CComponentTransform()
{
	mModelMatrix = glm::mat4(1.0f);
	mPosition = glm::vec2(0.0f);
	mScale = glm::vec2(1.0f);
	mRotation = 0.0f;
}

void CComponentTransform::UpdateModelMatrix()
{
	const auto cos = glm::cos( glm::radians( mRotation ) );
	const auto sin = glm::sin( glm::radians( mRotation ) );

	mModelMatrix[3][0] = mPosition.x;
	mModelMatrix[3][1] = mPosition.y;
	mModelMatrix[0][0] = mScale.x*cos;
	mModelMatrix[1][1] = mScale.y*cos;
	mModelMatrix[0][1] = -sin;
	mModelMatrix[1][0] = sin;
}
