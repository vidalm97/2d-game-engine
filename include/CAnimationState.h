#ifndef CANIMATION_STATE__H
#define CANIMATION_STATE__H

#include "CTextureRenderer.h"

#include <string>
#include <vector>

class CAnimationState
{
public:
	CAnimationState( const std::string aName = "New state" );
	~CAnimationState() = default;

	void AddNotTextured( const float aSpriteTime = 0.3f );
	void AddTextured( const std::string& aTexturePath, const float aSpriteTime = 0.3f );
	void AttachTexture( const unsigned int aSpriteIndex, const std::string& aTexturePath );

	const std::string& GetName() const;
	void SetName( const std::string& aName );

	std::vector<std::pair<CTextureRenderer,float>>& GetSprites();
	const bool HasSprites() const;
	const CTextureRenderer& GetCurrentSprite() const;
	void RenderCurrentSprite() const;
	void AddTime( const float aAddingTime );

private:
	std::string mName;
	int mCurrentSprite;
	float mAccumulatedTime;
	std::vector<std::pair<CTextureRenderer,float>> mSprites;
};

#endif
