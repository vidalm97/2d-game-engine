#include "Modules/CModuleEditor.h"

#include "CApplication.h"
#include "CComponentRenderer.h"
#include "CComponentTransform.h"
#include "CGameObject.h"
#include "Modules/CModuleRenderer.h"
#include "Modules/CModuleWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

bool CModuleEditor::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL( App->mWindow->mWindow, true );
	ImGui_ImplOpenGL3_Init( "#version 130" );

	return true;
}

bool CModuleEditor::Update()
{
	SetUpDockingSpace();
	RenderHierarchyPanel();
	RenderGameObjectPanel();
	RenderScenePanel();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if( mResizedPanel )
	{
		mResizedPanel = false;
		App->mRenderer->ResizeSceneFramebuffer( mScenePanelSize.x, mScenePanelSize.y );
	}
	return true;
}

bool CModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	return true;
}

bool CModuleEditor::Clear()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void CModuleEditor::SetUpDockingSpace()
{
	int windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos( viewport->Pos );
	ImGui::SetNextWindowSize( viewport->Size );
	ImGui::SetNextWindowViewport( viewport->ID );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGui::Begin( "Docking", NULL, windowFlags );
	ImGui::PopStyleVar( 3 );
	ImGui::DockSpace( ImGui::GetID("DockSpace"), ImVec2( 0.0f, 0.0f ), ImGuiDockNodeFlags_PassthruCentralNode );
	ImGui::End();
}

void CModuleEditor::RenderHierarchyPanel()
{
	ImGui::Begin( "Hierarchy" );

	if( ImGui::Button( "Create Game Object" ) )
	{
		App->mRenderer->GenerateGameObjectWithTexture("../resources/textures/bird.png");
	}

	for( int i = 0; i < App->mRenderer->mGameObjects.size(); ++i )
		if ( ImGui::Selectable( App->mRenderer->mGameObjects[i].mName.c_str() , mSelectedGO == i ) )
			mSelectedGO = i;

	ImGui::End();
}

void CModuleEditor::RenderGameObjectPanel()
{
	ImGui::Begin( "Inspector" );

	if( mSelectedGO == -1)
	{
		ImGui::End();
		return;
	}

	ImGui::Text( "Transform" );

	if( ImGui::DragFloat2("Position", (float*)&App->mRenderer->mGameObjects[mSelectedGO].mComponentTransform->mPosition.x, 0.1f, -200.0f, 200.0f, "%0.1f") )
		App->mRenderer->mGameObjects[mSelectedGO].mComponentTransform->UpdateModelMatrix();
	if( ImGui::DragFloat2("Scale", (float*)&App->mRenderer->mGameObjects[mSelectedGO].mComponentTransform->mScale.x, 0.1f, 0.1f, 200.0f, "%0.1f") )
		App->mRenderer->mGameObjects[mSelectedGO].mComponentTransform->UpdateModelMatrix();
	if( ImGui::DragFloat("Rotation", (float*)&App->mRenderer->mGameObjects[mSelectedGO].mComponentTransform->mRotation, 0.1f, -180.0f, 180.0f, "%0.1f") )
		App->mRenderer->mGameObjects[mSelectedGO].mComponentTransform->UpdateModelMatrix();

	if( App->mRenderer->mGameObjects[mSelectedGO].mComponentRenderer && App->mRenderer->mGameObjects[mSelectedGO].mComponentRenderer->HasTexture() )
	{
		const auto aspectRatio = App->mRenderer->mGameObjects[mSelectedGO].mComponentRenderer->GetTextureWidth()/App->mRenderer->mGameObjects[mSelectedGO].mComponentRenderer->GetTextureHeight();
		auto size = aspectRatio > 1.0f ? std::min( ImGui::GetWindowSize().x/aspectRatio, ImGui::GetWindowSize().y ) :
				std::min( ImGui::GetWindowSize().x, ImGui::GetWindowSize().y/aspectRatio );
		size -= 20.0f;

		ImGui::Text( "Sprite" );
		ImGui::Text( "Dimensions = %f x %f", App->mRenderer->mGameObjects[mSelectedGO].mComponentRenderer->GetTextureWidth(),
				App->mRenderer->mGameObjects[mSelectedGO].mComponentRenderer->GetTextureHeight() );
		ImGui::Image( (void*)(intptr_t)App->mRenderer->mGameObjects[mSelectedGO].mComponentRenderer->GetTextureId(),
				ImVec2( size*aspectRatio, size ), ImVec2( 0, 1 ), ImVec2( 1, 0 ), ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ), ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
	}

	ImGui::End();
}

void CModuleEditor::RenderScenePanel()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin( "Scene", &mShowScene, window_flags );

	auto scenePanelSize = ImGui::GetContentRegionAvail();
	if( glm::vec2( scenePanelSize.x, scenePanelSize.y ) != mScenePanelSize )
	{
		mScenePanelSize = glm::vec2( scenePanelSize.x, scenePanelSize.y );
		mResizedPanel = true;
	}

	ImGui::Image( (void*)(intptr_t)App->mRenderer->mSceneFramebufferTexture, scenePanelSize, ImVec2(0, 1), ImVec2(1, 0) );

	ImGui::End();
}
