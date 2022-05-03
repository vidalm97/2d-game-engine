#ifndef CTEXTURE__H
#define CTEXTURE__H

class CTexture
{
public:
	CTexture() = default;
	CTexture( const unsigned int aId, const float aWidth, const float aHeight, const int aChannelsNum );
	~CTexture() = default;

	unsigned int GetId() const;
	float GetWidth() const;
	float GetHeight() const;

private:
	unsigned int mId;
	float mWidth;
	float mHeight;
	int mChannelsNum;
};

#endif
