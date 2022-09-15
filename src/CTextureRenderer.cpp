#include "CTextureRenderer.h"

#include "CApplication.h"
#include "CTexture.h"
#include "Modules/CModuleResourceManager.h"

#include "glad/glad.h"
#include "glm.hpp"
#include "GLFW/glfw3.h"

CTextureRenderer::CTextureRenderer( const std::string& aTexturePath ) :
	mScaleDeviation(1.0f)
{
	mTexture = App->mResourceManager->CreateTexture( aTexturePath );
	UpdateVerticesData();
}

const CTexture* CTextureRenderer::GetTexture() const
{
	return mTexture;
}

const std::string& CTextureRenderer::GetTexturePath() const
{
	return mTexturePath;
}

const float CTextureRenderer::GetScaleDeviation() const
{
	return mScaleDeviation;
}

bool CTextureRenderer::AttachTexture( const std::string& aTexturePath )
{
	delete mTexture;

	mTexturePath = aTexturePath;
	mTexture = App->mResourceManager->CreateTexture( aTexturePath );

	UpdateVerticesData();

	return true;
}

void CTextureRenderer::UpdateVerticesData()
{
	const float ratio = mTexture->GetWidth() > mTexture->GetHeight() ?
			mTexture->GetHeight()/mTexture->GetWidth() : mTexture->GetWidth()/mTexture->GetHeight();
	glm::vec2 pos = mTexture->GetWidth() > mTexture->GetHeight() ? glm::vec2( 0.5f, ratio*0.5f ) : glm::vec2( ratio*0.5f, 0.5f );

	const auto proportionalSize = mTexture->GetWidth()*mTexture->GetHeight()/MAX_TEXTURE_SIZE;
	mScaleDeviation = glm::pow( proportionalSize, 0.5f );
	pos.x *= mScaleDeviation;
	pos.y *= mScaleDeviation;

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

void CTextureRenderer::Render() const
{
	if( !mTexture )
		return;

	glBindTexture( GL_TEXTURE_2D, mTexture->GetId() );

	glBindVertexArray( mVAO );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
}
