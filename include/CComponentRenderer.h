#ifndef CCOMPONENTRENDERER__H
#define CCOMPONENTRENDERER__H

#include "CSerializator.h"

#include "glm.hpp"

#include <string>

class CTexture;

class CComponentRenderer
{
public:
	CComponentRenderer() = default;
	CComponentRenderer( const std::string& aTexturePath, const glm::vec3& aBackColor );
	~CComponentRenderer() = default;

	bool AttachTexture( const std::string& aTexturePath );
	void UpdateVerticesData();
	void RenderTexture() const;
	bool HasTexture() const;

	unsigned int GetTextureId() const;
	float GetTextureWidth() const;
	float GetTextureHeight() const;
	const glm::vec3& GetBackColor() const;

	void Serialize( CSerializator::json& aJson ) const;
	void Deserialize( const CSerializator::json& aJson );

private:
	CTexture* mTexture = nullptr;
	std::string mTexturePath;

	// Used only for mouse picking
	glm::vec3 mBackColor = glm::vec3(0,0,0);

	unsigned int mVBO;
	unsigned int mVAO;
};

#endif
