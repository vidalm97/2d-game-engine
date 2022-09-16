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

	const glm::vec2& GetPosition() const;
	const glm::vec2& GetScale() const;
	const float& GetRotation() const;

	void SetPosition( const glm::vec2& aPosition );
	void SetScale( const glm::vec2& aScale );
	void SetRotation( const float aRotation );

	const glm::mat4& GetModelMatrix() const;
	void UpdateModelMatrix();

	void Serialize( CSerializator::json& aJson ) const;
	void Deserialize( const CSerializator::json& aJson );

private:
	glm::vec2 mPosition;
	glm::vec2 mScale;
	float mRotation;

	glm::mat4 mModelMatrix;
};

#endif
