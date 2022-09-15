#ifndef CCOMPONENT_ANIMATION__H
#define CCOMPONENT_ANIMATION__H

#include "AComponent.h"
#include "CAnimationState.h"

class CComponentAnimation : public AComponent
{
public:
	CComponentAnimation( const bool aActive = true );
	~CComponentAnimation() = default;

	void AddAnimationState();
	std::vector<CAnimationState>& GetAnimationStates();
	CAnimationState& GetCurrentAnimationState();
	bool HasAnimationStates() const;

	void AddNotTextured( const unsigned int aAnimationStateIndex, const float aSpriteTime = 0.3f );
	void AddTextured( const unsigned int aAnimationStateIndex, const std::string& aTexturePath, const float aSpriteTime = 0.3f );
	void AttachTexture( const unsigned int aAnimationStateIndex, const unsigned int aSpriteIndex, const std::string& aTexturePath );

private:
	std::vector<CAnimationState> mAnimationStates;
	unsigned int mCurrentAnimationState;
};

#endif
