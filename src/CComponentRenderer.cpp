#include "CComponentRenderer.h"

#include "CApplication.h"
#include "CTexture.h"
#include "Modules/CModuleResourceManager.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

CComponentRenderer::CComponentRenderer( const std::string& aTexturePath )
{
	AttachTexture( aTexturePath );
}

bool CComponentRenderer::AttachTexture( const std::string& aTexturePath )
{
	delete mTexture;
	mTexture = App->mResourceManager->CreateTexture( aTexturePath );

	UpdateVerticesData();

	return true;
}

void CComponentRenderer::UpdateVerticesData()
{
	const auto ratio = float(GetTextureWidth())/float(GetTextureHeight());
	float vertices[] = {
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		ratio, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		ratio, 1.0f, 0.0f, 1.0f, 1.0f,
		ratio, 0.0f, 0.0f, 1.0f, 0.0f
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
