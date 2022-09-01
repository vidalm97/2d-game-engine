#include "Modules/CModuleResourceManager.h"

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
