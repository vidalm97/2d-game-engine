#include "CTexture.h"

CTexture::CTexture( const unsigned int aId, const float aWidth, const float aHeight, const int aChannelsNum ) :
	mId(aId),
	mWidth(aWidth),
	mHeight(aHeight),
	mChannelsNum(aChannelsNum)
{
}

unsigned int CTexture::GetId() const
{
	return mId;
}

float CTexture::GetWidth() const
{
	return mWidth;
}

float CTexture::GetHeight() const
{
	return mHeight;
}
