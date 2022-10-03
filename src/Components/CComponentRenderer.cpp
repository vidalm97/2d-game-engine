#include "CComponentRenderer.h"

#include "CApplication.h"
#include "CTexture.h"
#include "CTextureRenderer.h"
#include "Modules/CModuleResourceManager.h"

#include "glad/glad.h"
#include "glm.hpp"
#include "GLFW/glfw3.h"
#include "stb_image.h"

const glm::vec3 GenerateUniqueColor( unsigned int aUniqueId )
{
	int r = (aUniqueId & 0x000000FF) >>  0;
	int g = (aUniqueId & 0x0000FF00) >>  8;
	int b = (aUniqueId & 0x00FF0000) >> 16;
	return glm::vec3( r, g, b );
}

CComponentRenderer::CComponentRenderer( unsigned int aUniqueColorID, const bool aActive ) :
	AComponent( RENDERER, aActive ),
	mBackColor( GenerateUniqueColor( aUniqueColorID ) ),
	mTextureRenderer( nullptr )
{
}

CComponentRenderer::CComponentRenderer( const std::string& aTexturePath, unsigned int aUniqueColorID, const bool aActive ) :
	AComponent( RENDERER, aActive ),
	mBackColor( GenerateUniqueColor( aUniqueColorID ) ),
	mTextureRenderer( nullptr )
{
	AttachTexture( aTexturePath );
}

std::unique_ptr<AComponent> CComponentRenderer::Clone() const
{
	return std::unique_ptr<AComponent>(new CComponentRenderer(*this));
}

bool CComponentRenderer::AttachTexture( const std::string& aTexturePath )
{
	if( !mTextureRenderer )
		mTextureRenderer = new CTextureRenderer( aTexturePath );
	else
		mTextureRenderer->AttachTexture( aTexturePath );

	return true;
}

unsigned int CComponentRenderer::GetTextureId() const
{
	return mTextureRenderer->GetTexture()->GetId();
}

float CComponentRenderer::GetTextureWidth() const
{
	return mTextureRenderer->GetTexture()->GetWidth();
}

float CComponentRenderer::GetTextureHeight() const
{
	return mTextureRenderer->GetTexture()->GetHeight();
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
	return mTextureRenderer && mTextureRenderer->GetTexture();
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
