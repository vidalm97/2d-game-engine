#include "CAnimationState.h"

#include "CApplication.h"
#include "Modules/CModuleResourceManager.h"

CAnimationState::CAnimationState( const std::string aName ) :
	mName( aName ),
	mCurrentSprite(0),
	mAccumulatedTime(0.0f)
{
}

void CAnimationState::AddNotTextured( const float aSpriteTime )
{
	mSprites.push_back( { CTextureRenderer(), aSpriteTime } );
}

void CAnimationState::AddTextured( const std::string& aTexturePath, const float aSpriteTime )
{
	mSprites.push_back( { CTextureRenderer( aTexturePath ), aSpriteTime } );
}

void CAnimationState::AttachTexture( const unsigned int aSpriteIndex, const std::string& aTexturePath )
{
	mSprites[aSpriteIndex].first.AttachTexture( aTexturePath );
}

const std::string& CAnimationState::GetName() const
{
	return mName;
}

void CAnimationState::SetName( const std::string& aName )
{
	mName = aName;
}

void CAnimationState::AddTime( const float aAddingTime )
{
	mAccumulatedTime = mAccumulatedTime + aAddingTime;
	if( mSprites[mCurrentSprite].second < mAccumulatedTime )
	{
		mAccumulatedTime = 0.0f;
		mCurrentSprite = mCurrentSprite < mSprites.size()-1 ? mCurrentSprite+1 : 0;
	}
}

const bool CAnimationState::HasSprites() const
{
	return mSprites.size() > 0;
}

std::vector<std::pair<CTextureRenderer,float>>& CAnimationState::GetSprites()
{
	return mSprites;
}

const CTextureRenderer& CAnimationState::GetCurrentSprite() const
{
	return mSprites[mCurrentSprite].first;
}

void CAnimationState::RenderCurrentSprite() const
{
	mSprites[mCurrentSprite].first.Render();
}
