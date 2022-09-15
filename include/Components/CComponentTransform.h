#ifndef CCOMPONENTTRANSFORM__H
#define CCOMPONENTTRANSFORM__H

#include "AComponent.h"
#include "CSerializator.h"

#include "glm.hpp"

class CComponentTransform : public AComponent
{
public:
	CComponentTransform( const bool aActive = true );
	~CComponentTransform() = default;

	void UpdateModelMatrix();

	void Serialize( CSerializator::json& aJson ) const;
	void Deserialize( const CSerializator::json& aJson );

	glm::mat4 mModelMatrix;
	glm::vec2 mPosition;
	glm::vec2 mScale;
	float mRotation;
};

#endif
