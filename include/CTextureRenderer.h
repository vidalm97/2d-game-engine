#ifndef CTEXTURE_RENDERER__H
#define CTEXTURE_RENDERER__H

#include <string>
#include <vector>

class CTexture;

class CTextureRenderer
{
public:
	CTextureRenderer() = default;
	CTextureRenderer( const std::string& aTexturePath );
	~CTextureRenderer() = default;

	const CTexture* GetTexture() const;
	const std::string& GetTexturePath() const;
	const float GetScaleDeviation() const;

	bool AttachTexture( const std::string& aTexturePath );
	void Render() const;

private:
	void UpdateVerticesData();

	CTexture* mTexture;
	std::string mTexturePath;
	float mScaleDeviation;

	std::vector<float> mVertices;
	unsigned int mVBO;
	unsigned int mVAO;
};

#endif
