#include "Modules/CModuleEditor.h"

#include "CApplication.h"
#include "CComponentAnimation.h"
#include "CComponentBoxCollider.h"
#include "CComponentRenderer.h"
#include "CComponentTransform.h"
#include "CGameObject.h"
#include "CTexture.h"

#include "Modules/CModuleRenderer.h"
#include "Modules/CModuleResourceManager.h"
#include "Modules/CModuleSceneManager.h"
#include "Modules/CModuleWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

CModuleEditor::CModuleEditor()
{
	mCurrentDirectory = std::filesystem::path("../resources");
}

bool CModuleEditor::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL( App->mWindow->mWindow, true );
	ImGui_ImplOpenGL3_Init( "#version 130" );

	mPlayIcon = App->mResourceManager->CreateTexture( "../engine_resources/icons/play.png" );
	mPauseIcon = App->mResourceManager->CreateTexture( "../engine_resources/icons/pause.png" );
	mStopIcon = App->mResourceManager->CreateTexture( "../engine_resources/icons/stop.png" );

	mDirectoryIcon = App->mResourceManager->CreateTexture( "../engine_resources/icons/directory.png" );
	mFileIcon = App->mResourceManager->CreateTexture( "../engine_resources/icons/file.png" );

	return true;
}

bool CModuleEditor::Update()
{
	SetUpDockingSpace();
	RenderSerializationPanel();
	RenderHierarchyPanel();
	RenderGameObjectPanel();
	RenderGameCameraPanel();
	RenderScenePanel();
	RenderGameControlPanel();
	RenderResourcePanel();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if( mResizedScenePanel )
	{
		mResizedScenePanel = false;
		App->mRenderer->ResizeSceneFramebuffer( mScenePanelSize.x, mScenePanelSize.y );
	}

	if( mResizedGamePanel )
	{
		mResizedGamePanel = false;
		App->mRenderer->ResizeGameFramebuffer( mGamePanelSize.x, mGamePanelSize.y );
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

	delete mPlayIcon;
	delete mPauseIcon;
	delete mStopIcon;

	delete mDirectoryIcon;
	delete mFileIcon;

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

void CModuleEditor::RenderSerializationPanel()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin( "##Serialization panel", nullptr, windowFlags );

	ImGui::Dummy(ImVec2(0.0f,3.0f));
	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 20.0f );
	if( ImGui::Button( "Save scene" ) )
		App->mResourceManager->SerializeScene();
	ImGui::SameLine();
	if( ImGui::Button( "Load scene" ) )
		App->mResourceManager->DeserializeScene();

	ImGui::End();

	ImGui::PopStyleVar(3);
}

void CModuleEditor::RenderHierarchyPanel()
{
	ImGui::Begin( "Hierarchy" );

	if( ImGui::Button( "Create Game Object" ) )
	{
		App->mSceneManager->CreateGameObject();
	}

	for( int i = 0; i < App->mSceneManager->GetGameObjects().size(); ++i )
		if ( ImGui::Selectable( App->mSceneManager->GetGameObjects()[i].GetName().c_str(), App->mSceneManager->mSelectedGOIndex == i ) )
		{
			App->mSceneManager->SetSelectedGO( i );
		}

	ImGui::End();
}

void CModuleEditor::RenderGameObjectPanel()
{
	ImGui::Begin( "Inspector" );

	if( !App->mSceneManager->HasSelectedGO() )
	{
		ImGui::End();
		return;
	}

	ImGui::Dummy( ImVec2( 0, 1 ) );
	if( ImGui::CollapsingHeader( "Transform", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		auto* transform = static_cast<CComponentTransform*>(App->mSceneManager->GetSelectedGO().GetComponent<TRANSFORM>());
		if( ImGui::DragFloat2("Position", (float*)&transform->GetPosition().x, 0.1f, -200.0f, 200.0f, "%0.1f") )
		{
			transform->UpdateModelMatrix();
			App->mSceneManager->UpdateGizmoPosition();
		}
		if( ImGui::DragFloat2("Scale", (float*)&transform->GetScale().x, 0.1f, 0.1f, 200.0f, "%0.1f") )
			transform->UpdateModelMatrix();
		if( ImGui::DragFloat("Rotation", (float*)&transform->GetRotation(), 0.1f, -180.0f, 180.0f, "%0.1f") )
			transform->UpdateModelMatrix();
	}

	if( static_cast<CComponentRenderer*>(App->mSceneManager->GetSelectedGO().GetComponent<RENDERER>()) )
		RenderRendererPanel();

	if( static_cast<CComponentAnimation*>(App->mSceneManager->GetSelectedGO().GetComponent<ANIMATION>()) )
	{
		ImGui::Dummy( ImVec2( 0, 1 ) );
		RenderAnimationPanel();
	}

	if( static_cast<CComponentBoxCollider*>(App->mSceneManager->GetSelectedGO().GetComponent<BOX_COLLIDER>()) )
	{
		ImGui::Dummy( ImVec2( 0, 1 ) );
		RenderBoxColliderPanel();
	}

	ImGui::Dummy( ImVec2( 0, 1 ) );
	RenderAddComponentPanel();

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
		mResizedScenePanel = true;
		App->mRenderer->mSceneViewportXPos = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x;
		App->mRenderer->mSceneViewportYPos = ImGui::GetWindowPos().y+ ImGui::GetWindowContentRegionMin().y;
	}

	ImGui::Image( (void*)(intptr_t)App->mRenderer->mSceneFramebufferTexture, scenePanelSize, ImVec2(0, 1), ImVec2(1, 0) );

	ImGui::End();
}

void CModuleEditor::RenderGameCameraPanel()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin( "Game", &mShowGameCamera, window_flags );

	auto gamePanelSize = ImGui::GetContentRegionAvail();
	if( glm::vec2( gamePanelSize.x, gamePanelSize.y ) != mGamePanelSize )
	{
		mGamePanelSize = glm::vec2( gamePanelSize.x, gamePanelSize.y );
		mResizedGamePanel = true;
	}

	ImGui::Image( (void*)(intptr_t)App->mRenderer->mGameFramebufferTexture, gamePanelSize, ImVec2(0, 1), ImVec2(1, 0) );

	ImGui::End();
}

void CModuleEditor::RenderGameControlPanel()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::PushStyleColor( ImGuiCol_WindowBg,ImVec4(0.0f, 0.3f, 0.5f, 1.f));
	ImGui::Begin("##toolbar", nullptr, windowFlags );

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.f));

	const auto aspectRatio = mPlayIcon->GetWidth()/mPlayIcon->GetHeight();
	auto size = aspectRatio > 1.0f ? std::min( ImGui::GetContentRegionAvail().x/aspectRatio, ImGui::GetContentRegionAvail().y ) :
				std::min( ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y/aspectRatio );
	size -= 5.0f;

	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 0.5*ImGui::GetContentRegionAvail().x - 1.5f*size );

	if( ImGui::ImageButton( (void*)(intptr_t)mPlayIcon->GetId(), ImVec2( size*aspectRatio, size ), ImVec2( 0, 1 ), ImVec2( 1, 0 ) ) )
	{
		// TO DO: trigger game
	}

	ImGui::SameLine();
	if( ImGui::ImageButton( (void*)(intptr_t)mPauseIcon->GetId(), ImVec2( size*aspectRatio, size ), ImVec2( 0, 1 ), ImVec2( 1, 0 ) ) )
	{
		// TO DO: pause game
	}

	ImGui::SameLine();
	if( ImGui::ImageButton( (void*)(intptr_t)mStopIcon->GetId(), ImVec2( size*aspectRatio, size ), ImVec2( 0, 1 ), ImVec2( 1, 0 ) ) )
	{
		// TO DO: stop game
	}

	ImGui::PopStyleColor(4);
	ImGui::PopStyleVar(3);
	ImGui::End();
}

void CModuleEditor::RenderResourcePanel()
{
	static const std::filesystem::path RESOURCES_PATH = "../resources";

	ImGui::Begin( "Resources", nullptr );

	if( mCurrentDirectory != std::filesystem::path("../resources") )
		if( ImGui::Button("<-"))
			mCurrentDirectory = "../resources";

	int i = 0;
	const auto panelSize = ImGui::GetContentRegionAvail().x;
	for( const auto& directory : std::filesystem::directory_iterator(mCurrentDirectory) )
	{
		const auto& path = directory.path();
		auto relativePath = std::filesystem::relative( path, RESOURCES_PATH );
		const auto& filename = relativePath.filename().string();

		const auto imageID = directory.is_directory() ? (void*)(intptr_t)mDirectoryIcon->GetId() : (void*)(intptr_t)mFileIcon->GetId();

		ImGui::PushID(filename.c_str());
		ImGui::BeginGroup();

		if( ImGui::ImageButton( imageID, ImVec2( 90,90 ), ImVec2( 0, 1 ), ImVec2( 1, 0 ) ) )
			if( directory.is_directory() )
				mCurrentDirectory /= path.filename();

		if( ImGui::BeginDragDropSource() )
		{
			const char* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload( "Resource_panel", itemPath, (strlen(itemPath)+1)*sizeof(char) );
			ImGui::EndDragDropSource();
		}

		ImGui::PushTextWrapPos(100*++i);
		ImGui::TextWrapped("%s", filename.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndGroup();
		if( 100*i < panelSize )
			ImGui::SameLine();
		else
			i = 0;
		ImGui::PopID();
	}

	ImGui::End();
}

void CModuleEditor::RenderAddComponentPanel()
{
	std::vector<std::string> items = { "Renderer", "Box Collider", "Animation" };

	if ( ImGui::BeginCombo("##Add Component", "Add Component", ImGuiComboFlags_NoArrowButton ) )
	{
		for ( int n = 0; n < items.size(); ++n )
			if (ImGui::Selectable(items[n].c_str() ) )
				switch( n )
				{
					case 0:
					{
						App->mSceneManager->GetSelectedGO().PushComponent<CComponentRenderer>(
								CComponentRenderer( App->mSceneManager->GetSelectedGO().GetID() ) );
						break;
					}
					case 1:
					{
						auto* transform = static_cast<CComponentTransform*>(App->mSceneManager->GetSelectedGO().GetComponent<TRANSFORM>());
						auto* renderer = static_cast<CComponentRenderer*>(App->mSceneManager->GetSelectedGO().GetComponent<RENDERER>());
						App->mSceneManager->GetSelectedGO().PushComponent<CComponentBoxCollider>( CComponentBoxCollider( transform->GetPosition(),
								renderer && renderer->HasTexture() ?
								glm::vec2( renderer->GetTextureScaleDeviation()*transform->GetScale().x*renderer->GetTextureWidth()/renderer->GetTextureHeight(),
								renderer->GetTextureScaleDeviation()*transform->GetScale().y ) : glm::vec2( 10, 10 ),  false ) );
						break;
					}
					case 2:
						App->mSceneManager->GetSelectedGO().PushComponent<CComponentAnimation>( CComponentAnimation() );
						break;
				}

		ImGui::EndCombo();
	}
}

void CModuleEditor::RenderRendererPanel()
{
	if( ImGui::CollapsingHeader( "Sprite renderer", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		auto renderer = static_cast<CComponentRenderer*>(App->mSceneManager->GetSelectedGO().GetComponent<RENDERER>());
		if( renderer->HasTexture() )
		{
			const auto aspectRatio = renderer->GetTextureWidth()/renderer->GetTextureHeight();
			auto size = aspectRatio > 1.0f ? std::min( ImGui::GetWindowSize().x/aspectRatio, ImGui::GetWindowSize().y ) :
					std::min( ImGui::GetWindowSize().x, ImGui::GetWindowSize().y/aspectRatio );
			size /= 2;

			ImGui::Dummy( ImVec2( 0, 1 ) );

			ImGui::Text( "Dimensions = %d x %d", int(renderer->GetTextureWidth()), int(renderer->GetTextureHeight()) );
			ImGui::Image( (void*)(intptr_t)renderer->GetTextureId(),
					ImVec2( size*aspectRatio, size ), ImVec2( 0, 1 ), ImVec2( 1, 0 ), ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ), ImVec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
		}
		else
		{
			ImGui::Text("Drop Texture");
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource_panel"))
			{
				auto path = (const char*)payload->Data;
				renderer->AttachTexture(path);
			}
			ImGui::EndDragDropTarget();
		}
	}
}

void CModuleEditor::RenderBoxColliderPanel()
{
	ImGui::CollapsingHeader( "Box Collider", ImGuiTreeNodeFlags_DefaultOpen );

	auto* boxCollider = static_cast<CComponentBoxCollider*>(App->mSceneManager->GetSelectedGO().GetComponent<BOX_COLLIDER>());
	auto center = boxCollider->GetCenter();
	if( ImGui::DragFloat2("Center", (float*)&center.x, 0.1f, -200.0f, 200.0f, "%0.01f") )
		boxCollider->SetCenter( center );


	auto size = boxCollider->GetSize();
	if( ImGui::DragFloat2("Size", (float*)&size.x, 0.1f, -200.0f, 200.0f, "%0.01f") )
		boxCollider->SetSize( size );
}

void CModuleEditor::RenderAnimationPanel()
{
	ImGui::CollapsingHeader( "Animation", ImGuiTreeNodeFlags_DefaultOpen );

	auto* animation = static_cast<CComponentAnimation*>(App->mSceneManager->GetSelectedGO().GetComponent<ANIMATION>());
	if( ImGui::Button( "Add state" ) )
		animation->AddAnimationState();

	int stateCount = 0;
	for( auto& animationState : animation->GetAnimationStates() )
	{
		static int size = 64;
		ImGui::PushID( "state"+stateCount);
		++stateCount;
		std::string name = animationState.GetName();
		if(ImGui::ResizableInputText( "##", &name, ImGuiInputTextFlags_CallbackResize ))
			animationState.SetName( name );

		if( ImGui::Button( "Add sprite" ) )
			animationState.AddNotTextured();

		int spriteCount = 0;
		for( auto& sprite : animationState.GetSprites() )
		{
			if( sprite.first.GetTexture() )
			{
				const auto aspectRatio = sprite.first.GetTexture()->GetWidth()/sprite.first.GetTexture()->GetHeight();

				ImGui::Text( "Sprite" );
				ImGui::Image( (void*)(intptr_t)sprite.first.GetTexture()->GetId(), ImVec2( size*aspectRatio, size ), ImVec2( 0, 1 ),
						ImVec2( 1, 0 ), ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ), ImVec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
			}
			else
			{
				ImGui::Text("Drop sprite");
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource_panel"))
				{
					auto path = (const char*)payload->Data;
					sprite.first.AttachTexture(path);
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			ImGui::PushID( stateCount+"sprite"+spriteCount);
			++spriteCount;
			ImGui::DragFloat("##", (float*)&sprite.second, 0.1f, 0.01f, 180.0f, "%0.1f");
			ImGui::PopID();
		}
		ImGui::PopID();
	}
}
