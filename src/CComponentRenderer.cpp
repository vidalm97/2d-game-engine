#include "CComponentRenderer.h"

#include "CApplication.h"
#include "CTexture.h"
#include "Modules/CModuleResourceManager.h"

#include "glad/glad.h"
#include "glm.hpp"
#include "GLFW/glfw3.h"
#include "stb_image.h"

CComponentRenderer::CComponentRenderer( const std::string& aTexturePath, const glm::vec3& aBackColor ) :
	mTexturePath( aTexturePath ),
	mBackColor( aBackColor )
{
	AttachTexture( aTexturePath );
}

bool CComponentRenderer::AttachTexture( const std::string& aTexturePath )
{
	delete mTexture;

	mTexturePath = aTexturePath;
	mTexture = App->mResourceManager->CreateTexture( aTexturePath );

	UpdateVerticesData();

	return true;
}

void CComponentRenderer::UpdateVerticesData()
{
	const float ratio = GetTextureWidth() > GetTextureHeight() ? GetTextureHeight()/GetTextureWidth() : GetTextureWidth()/GetTextureHeight();
	const glm::vec2 pos = GetTextureWidth() > GetTextureHeight() ? glm::vec2( 0.5f, ratio*0.5f ) : glm::vec2( ratio*0.5f, 0.5f );

	float vertices[] = {
		-pos.x, pos.y, 0.0f, 0.0f, 1.0f,
		pos.x, -pos.y, 0.0f, 1.0f, 0.0f,
		-pos.x, -pos.y, 0.0f, 0.0f, 0.0f,

		-pos.x, pos.y, 0.0f, 0.0f, 1.0f,
		pos.x, pos.y, 0.0f, 1.0f, 1.0f,
		pos.x, -pos.y, 0.0f, 1.0f, 0.0f
	};

	glGenBuffers( 1, &mVBO );
	glGenVertexArrays( 1, &mVAO );
	glBindVertexArray( mVAO );
	glBindBuffer( GL_ARRAY_BUFFER, mVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0 );
	glEnableVertexAttribArray( 0 );

	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)) );
	glEnableVertexAttribArray( 1 );
}

unsigned int CComponentRenderer::GetTextureId() const
{
	return mTexture->GetId();
}

float CComponentRenderer::GetTextureWidth() const
{
	return mTexture->GetWidth();
}

float CComponentRenderer::GetTextureHeight() const
{
	return mTexture->GetHeight();
}

const glm::vec3& CComponentRenderer::GetBackColor() const
{
	return mBackColor;
}

bool CComponentRenderer::HasTexture() const
{
	return mTexture!=nullptr;
}

void CComponentRenderer::RenderTexture() const
{
	glBindTexture( GL_TEXTURE_2D, GetTextureId() );

	glBindVertexArray( mVAO );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
}

void CComponentRenderer::Serialize( CSerializator::json& aJson ) const
{
	aJson["texturePath"] = mTexturePath;
	aJson["BackColor"] = {{"x", mBackColor.x}, {"y", mBackColor.y}, {"z", mBackColor.z}};
}

void CComponentRenderer::Deserialize( const CSerializator::json& aJson )
{
	mBackColor.x = aJson.at("BackColor")["x"];
	mBackColor.y = aJson.at("BackColor")["y"];
	mBackColor.z = aJson.at("BackColor")["z"];

	AttachTexture( aJson.at("texturePath") );
}
