#ifndef ACOMPONENT__H
#define ACOMPONENT__H

#include <memory>

enum COMPONENT_TYPE
{
	TRANSFORM,
	RENDERER,
	BOX_COLLIDER,
	ANIMATION
};

class AComponent
{
public:
	AComponent( const COMPONENT_TYPE& aType, const bool aActive );
	~AComponent() = default;

	virtual std::unique_ptr<AComponent> Clone() const = 0;

	const COMPONENT_TYPE& GetType() const;
	void SetActive( const bool aActive );
	bool IsActive() const;

private:
	COMPONENT_TYPE mType;
	bool mActive;
};

#endif
