#ifndef CTEXTURE__H
#define CTEXTURE__H

class CTexture
{
public:
	CTexture() = default;
	CTexture( const unsigned int aTextureId, const int aWidth, const int aHeight, const int aChannelsNum );
	~CTexture() = default;

	unsigned int mTextureId;
	int mWidth;
	int mHeight;
	int mChannelsNum;
};

#endif
