#ifndef CTEXTURE__H
#define CTEXTURE__H

class CTexture
{
public:
	CTexture() = default;
	CTexture( const int aWidth, const int aHeight, const int aChannelsNum );
	~CTexture() = default;

private:
	int mWidth;
	int mHeight;
	int mChannelsNum;
};

#endif
