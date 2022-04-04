#include "CApplication.h"

#include "AModule.h"
#include "CModuleWindow.h"

bool CApplication::Start()
{
	// Insert modules in mModules here
	mModules.push_back(new CModuleWindow( 640, 480 ));
	return true;
}

EAppStatus CApplication::Init() const
{
	for( const auto& module : mModules )
		if( !module->Init() )
			return EAppStatus::STOP;
	return EAppStatus::UPDATE;
}

EAppStatus CApplication::Update() const
{
	for( const auto& module : mModules )
		if( !module->Update() )
			return EAppStatus::STOP;
	return EAppStatus::UPDATE;
}

EAppStatus CApplication::Clear() const
{
	for( const auto& module : mModules )
		module->Clear();
	return EAppStatus::STOP;
}
