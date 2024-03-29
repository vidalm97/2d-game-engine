#include "CApplication.h"

#include "AModule.h"
#include "Modules/CModuleCamera.h"
#include "Modules/CModuleEditor.h"
#include "Modules/CModuleInput.h"
#include "Modules/CModuleRenderer.h"
#include "Modules/CModuleResourceManager.h"
#include "Modules/CModuleSceneManager.h"
#include "Modules/CModuleTime.h"
#include "Modules/CModuleWindow.h"

bool CApplication::Start()
{
	// Insert modules in mModules here
	mModules.push_back( mWindow = new CModuleWindow( 640, 480 ) );
	mModules.push_back( mTimer = new CModuleTime() );
	mModules.push_back( mCamera = new CModuleCamera() );
	mModules.push_back( mInput = new CModuleInput() );
	mModules.push_back( mRenderer = new CModuleRenderer() );
	mModules.push_back( mResourceManager = new CModuleResourceManager( "../resources" ) );
	mModules.push_back( mSceneManager = new CModuleSceneManager() );
	mModules.push_back( mEditor = new CModuleEditor() );

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
		if( !module->PreUpdate() )
			return EAppStatus::STOP;

	for( const auto& module : mModules )
		if( !module->Update() )
			return EAppStatus::STOP;

	for( const auto& module : mModules )
		if( !module->PostUpdate() )
			return EAppStatus::STOP;

	return EAppStatus::UPDATE;
}

EAppStatus CApplication::Clear() const
{
	for( const auto& module : mModules )
		module->Clear();
	return EAppStatus::STOP;
}
