#include "CApplication.h"

#include "AModule.h"
#include "CModuleCamera.h"
#include "CModuleEditor.h"
#include "CModuleInput.h"
#include "CModuleRenderer.h"
#include "CModuleWindow.h"

bool CApplication::Start()
{
	// Insert modules in mModules here
	mModules.push_back( mWindow = new CModuleWindow( 640, 480 ) );
	mModules.push_back( mCamera = new CModuleCamera( 0.001f ) );
	mModules.push_back( mEditor = new CModuleEditor() );
	mModules.push_back( mInput = new CModuleInput() );
	mModules.push_back( mRenderer = new CModuleRenderer() );

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
