#include "CTexture.h"

CTexture::CTexture( const unsigned int aTextureId, const int aWidth, const int aHeight, const int aChannelsNum ) :
	mTextureId(aTextureId),
	mWidth(aWidth),
	mHeight(aHeight),
	mChannelsNum(aChannelsNum)
{
}
