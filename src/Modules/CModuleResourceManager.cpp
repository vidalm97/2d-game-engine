#include "Modules/CModuleResourceManager.h"

#include "CTexture.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include <fstream>

CModuleResourceManager::CModuleResourceManager( const std::string& aResourcesPath ) : mResourcesPath( aResourcesPath )
{
}

bool CModuleResourceManager::Update()
{
	return true;
}

const std::string CModuleResourceManager::LoadFile( const std::string& aFilePath )
{
	std::ifstream ifs( mResourcesPath+"/"+aFilePath );
	std::string content( ( std::istreambuf_iterator<char>(ifs) ), ( std::istreambuf_iterator<char>() ) );

	return content;
}

CTexture* CModuleResourceManager::CreateTexture( const std::string& aTexturePath )
{
	unsigned int textureId;
	glGenTextures( 1, &textureId );

	glBindTexture( GL_TEXTURE_2D, textureId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load( (mResourcesPath+"/"+aTexturePath).c_str(), &width, &height, &nrChannels, 0 );
	if (data)
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
	else
	{
		return nullptr;
	}
	stbi_image_free( data );

	return new CTexture( textureId, width, height, nrChannels );
}
