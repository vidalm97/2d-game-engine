#ifndef C_MODULE_RESOURCE_MANAGER__H
#define C_MODULE_RESOURCE_MANAGER__H

#include "AModule.h"

#include <string>

class CModuleResourceManager : public AModule
{
public:
	CModuleResourceManager( const std::string& aResourcesPath );
	~CModuleResourceManager() = default;

	bool Update() override;

	const std::string LoadFile( const std::string& aFilePath );

private:
	std::string mResourcesPath;
};

#endif
