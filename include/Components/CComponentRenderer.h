#ifndef CCOMPONENTRENDERER__H
#define CCOMPONENTRENDERER__H

#include "AComponent.h"
#include "CSerializator.h"

#include "glm.hpp"

#include <string>

class CTextureRenderer;

class CComponentRenderer : public AComponent
{
public:
	CComponentRenderer( const bool aActive = true );
	CComponentRenderer( const std::string& aTexturePath, const glm::vec3& aBackColor, const bool aActive = true );
	~CComponentRenderer() = default;

	bool AttachTexture( const std::string& aTexturePath );
	void RenderTexture() const;
	bool HasTexture() const;

	unsigned int GetTextureId() const;
	float GetTextureWidth() const;
	float GetTextureHeight() const;
	const float GetTextureScaleDeviation() const;
	const glm::vec3& GetBackColor() const;

	void Serialize( CSerializator::json& aJson ) const;
	void Deserialize( const CSerializator::json& aJson );

private:
	CTextureRenderer* mTextureRenderer;

	// Used only for mouse picking
	glm::vec3 mBackColor = glm::vec3(0,0,0);
};

#endif
