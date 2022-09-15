#include "AComponent.h"

AComponent::AComponent( const COMPONENT_TYPE& aType, const bool aActive ) :
	mType( aType ),
	mActive( aActive )
{
}

const COMPONENT_TYPE& AComponent::GetType() const
{
	return mType;
}

void AComponent::SetActive( const bool aActive )
{
	mActive = aActive;
}

bool AComponent::IsActive() const
{
	return mActive;
}
