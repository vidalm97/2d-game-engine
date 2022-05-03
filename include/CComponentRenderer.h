#ifndef CCOMPONENTRENDERER__H
#define CCOMPONENTRENDERER__H

#include <string>

class CTexture;

class CComponentRenderer
{
public:
	CComponentRenderer() = default;
	CComponentRenderer( const std::string& aTexturePath );
	~CComponentRenderer() = default;

	bool AttachTexture( const std::string& aTexturePath );
	void UpdateVerticesData();
	void RenderTexture() const;
	bool HasTexture() const;

	unsigned int GetTextureId() const;
	float GetTextureWidth() const;
	float GetTextureHeight() const;

private:
	CTexture* mTexture = nullptr;

	unsigned int mVBO;
	unsigned int mVAO;
};

#endif
