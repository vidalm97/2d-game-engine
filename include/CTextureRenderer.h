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

	unsigned int GetTextureId() const;
	float GetTextureWidth() const;
	float GetTextureHeight() const;
	const std::string& GetTexturePath() const;
	const float GetScaleDeviation() const;

	bool HasTexture() const;
	bool AttachTexture( const std::string& aTexturePath );
	void UpdateVerticesData();
	void Render() const;

private:
	std::vector<float> mVertices;
	CTexture* mTexture;
	std::string mTexturePath;
	unsigned int mVBO;
	unsigned int mVAO;
	float mScaleDeviation;
};

#endif
