#ifndef CCOMPONENTTRANSFORM__H
#define CCOMPONENTTRANSFORM__H

#include "glm.hpp"

class CComponentTransform
{
public:
	CComponentTransform();
	~CComponentTransform() = default;

	void UpdateModelMatrix();

	glm::mat4 mModelMatrix;
	glm::vec2 mPosition;
	glm::vec2 mScale;
	float mRotation;
};

#endif
