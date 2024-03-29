#ifndef CCOMPONENT_BOX_COLLIDER__H
#define CCOMPONENT_BOX_COLLIDER__H

#include "AComponent.h"

#include "glm.hpp"

class CComponentBoxCollider : public AComponent
{
public:
	CComponentBoxCollider( const glm::vec2& aCenter, const glm::vec2& aSize, const bool aIsTrigger, const bool aActive = true );
	~CComponentBoxCollider() = default;

	std::unique_ptr<AComponent> Clone() const override;

	const glm::vec2& GetCenter() const;
	const glm::vec2& GetSize() const;
	const bool IsTrigger() const;

	void SetCenter( const glm::vec2& aCenter );
	void SetSize( const glm::vec2& aSize );
	void SetTrigger( const bool aIsTrigger );

private:
	glm::vec2 mCenter;
	glm::vec2 mSize;
	bool mIsTrigger;
};

#endif
