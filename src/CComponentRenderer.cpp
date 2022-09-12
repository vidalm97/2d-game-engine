#include "CComponentRenderer.h"

#include "CApplication.h"
#include "CTextureRenderer.h"
#include "Modules/CModuleResourceManager.h"

#include "glad/glad.h"
#include "glm.hpp"
#include "GLFW/glfw3.h"
#include "stb_image.h"

CComponentRenderer::CComponentRenderer( const std::string& aTexturePath, const glm::vec3& aBackColor ) :
	mBackColor( aBackColor )
{
	AttachTexture( aTexturePath );
}

bool CComponentRenderer::AttachTexture( const std::string& aTexturePath )
{
	mTextureRenderer = new CTextureRenderer( aTexturePath );

	return true;
}

void CComponentRenderer::UpdateVerticesData()
{
	mTextureRenderer->UpdateVerticesData();
}

unsigned int CComponentRenderer::GetTextureId() const
{
	return mTextureRenderer->GetTextureId();
}

float CComponentRenderer::GetTextureWidth() const
{
	return mTextureRenderer->GetTextureWidth();
}

float CComponentRenderer::GetTextureHeight() const
{
	return mTextureRenderer->GetTextureHeight();
}

const float CComponentRenderer::GetTextureScaleDeviation() const
{
	return mTextureRenderer->GetScaleDeviation();
}

const glm::vec3& CComponentRenderer::GetBackColor() const
{
	return mBackColor;
}

bool CComponentRenderer::HasTexture() const
{
	return mTextureRenderer != nullptr;
}

void CComponentRenderer::RenderTexture() const
{
	mTextureRenderer->Render();
}

void CComponentRenderer::Serialize( CSerializator::json& aJson ) const
{
	aJson["texturePath"] = mTextureRenderer->GetTexturePath();
	aJson["BackColor"] = {{"x", mBackColor.x}, {"y", mBackColor.y}, {"z", mBackColor.z}};
}

void CComponentRenderer::Deserialize( const CSerializator::json& aJson )
{
	mBackColor.x = aJson.at("BackColor")["x"];
	mBackColor.y = aJson.at("BackColor")["y"];
	mBackColor.z = aJson.at("BackColor")["z"];

	AttachTexture( aJson.at("texturePath") );
}
