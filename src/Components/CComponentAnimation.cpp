#include "CComponentAnimation.h"

CComponentAnimation::CComponentAnimation( const bool aActive ) :
	AComponent( ANIMATION, aActive )
{
}

void CComponentAnimation::AddAnimationState()
{
	mAnimationStates.push_back( CAnimationState() );
}

std::vector<CAnimationState>& CComponentAnimation::GetAnimationStates()
{
	return mAnimationStates;
}

CAnimationState& CComponentAnimation::GetCurrentAnimationState()
{
	return mAnimationStates[mCurrentAnimationState];
}

bool CComponentAnimation::HasAnimationStates() const
{
	return mAnimationStates.size() > 0;
}

void CComponentAnimation::AddNotTextured( const unsigned int aAnimationStateIndex, const float aSpriteTime )
{
	mAnimationStates[aAnimationStateIndex].AddNotTextured( aSpriteTime );
}

void CComponentAnimation::AddTextured( const unsigned int aAnimationStateIndex, const std::string& aTexturePath, const float aSpriteTime )
{
	mAnimationStates[aAnimationStateIndex].AddTextured( aTexturePath, aSpriteTime );
}

void CComponentAnimation::AttachTexture( const unsigned int aAnimationStateIndex, const unsigned int aSpriteIndex, const std::string& aTexturePath )
{
	mAnimationStates[aAnimationStateIndex].AttachTexture( aSpriteIndex, aTexturePath );
}
