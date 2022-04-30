#include "Modules/CModuleEditor.h"

#include "CApplication.h"
#include "CGameObject.h"
#include "Modules/CModuleRenderer.h"
#include "Modules/CModuleWindow.h"

#include "imgui.h"
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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	RenderHierarchyPanel();
	RenderGameObjectPanel();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}

bool CModuleEditor::Clear()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void CModuleEditor::RenderHierarchyPanel()
{
	ImGui::Begin( "Hierarchy" );

	if( ImGui::Button( "Create Game Object" ) )
	{
		App->mRenderer->GenerateGameObjectWithTexture("../resources/textures/test.jpg");
	}

	for( int i = 0; i < App->mRenderer->mGameObjects.size(); ++i )
		if ( ImGui::Selectable( App->mRenderer->mGameObjects[i].mName.c_str() , mSelectedGO == i ) )
			mSelectedGO = i;

	ImGui::End();
}

void CModuleEditor::RenderGameObjectPanel()
{
	ImGui::Begin( "Game Object" );

	if( mSelectedGO == -1)
	{
		ImGui::End();
		return;
	}

	if( ImGui::DragFloat2("Position", (float*)&App->mRenderer->mGameObjects[mSelectedGO].mPosition.x, 0.5f, 1.0f, 200.0f, "%.0f") )
		App->mRenderer->mGameObjects[mSelectedGO].UpdateModelMatrix();

	ImGui::End();
}