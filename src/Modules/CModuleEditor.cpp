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
#include "IconsFontAwesome.h"

CModuleEditor::CModuleEditor()
{
	mCurrentDirectory = std::filesystem::path("../resources");
}

bool CModuleEditor::Init()
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.Fonts->AddFontDefault();

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF( "../engine_resources/icons/font.otf", 12.0f, &icons_config, icons_ranges );

	ImGui_ImplGlfw_InitForOpenGL( App->mWindow->mWindow, true );
	ImGui_ImplOpenGL3_Init( "#version 130" );

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
	RenderGizmoPanel();

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
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin( ICON_FA_COMPACT_DISC " Scene Loader", nullptr, windowFlags );

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
	ImGui::Begin( ICON_FA_LIST " Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse );

	if( ImGui::Button( "Create Game Object" ) )
	{
		App->mSceneManager->CreateGameObject();
	}

	for( int i = 0; i < App->mSceneManager->GetGameObjects().size(); ++i )
	{
		const std::string label = ICON_FA_CUBE + std::string(" ") + App->mSceneManager->GetGameObjects()[i].GetName();
		if ( ImGui::Selectable( label.c_str(), App->mSceneManager->mSelectedGOIndex == i ) )
			App->mSceneManager->SetSelectedGO( i );
	}
	ImGui::End();
}

void CModuleEditor::RenderGameObjectPanel()
{
	ImGui::Begin( ICON_FA_INFO_CIRCLE " Inspector" );

	if( !App->mSceneManager->HasSelectedGO() )
	{
		ImGui::End();
		return;
	}

	ImGui::Dummy( ImVec2( 0, 1 ) );
	if( ImGui::CollapsingHeader( ICON_FA_ADJUST " Transform", ImGuiTreeNodeFlags_DefaultOpen ) )
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
	{
		ImGui::Dummy( ImVec2( 0, 1 ) );
		RenderRendererPanel();
	}

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

	ImGui::Dummy( ImVec2( 0, 5 ) );
	RenderAddComponentPanel();

	ImGui::End();
}

void CModuleEditor::RenderScenePanel()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin( ICON_FA_OBJECT_GROUP " Scene", nullptr, window_flags );

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

	ImGui::Begin( ICON_FA_GAMEPAD " Game", nullptr, window_flags );

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
	ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 0.5*ImGui::GetContentRegionAvail().x );

	if( ImGui::Button( ICON_FA_PLAY "##PlayButton" ) )
	{
		// TO DO: trigger game
	}

	ImGui::SameLine();
	if( ImGui::Button( ICON_FA_PAUSE "##PauseButton" ) )
	{
		// TO DO: pause game
	}

	ImGui::SameLine();
	if( ImGui::Button( ICON_FA_STOP "##StopButton" ) )
	{
		// TO DO: stop game
	}

	ImGui::End();
}

void CModuleEditor::RenderResourcePanel()
{
	static const std::filesystem::path RESOURCES_PATH = "../resources";

	ImGui::Begin( ICON_FA_FOLDER " Resources", nullptr );

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

		ImGui::SetWindowFontScale(1.5f);
		const auto icon = directory.is_directory() ? ICON_FA_FOLDER : ICON_FA_FILE;

		ImGui::PushID(filename.c_str());
		ImGui::BeginGroup();

		if( ImGui::Button( icon, ImVec2( 90,90 ) ) )
			if( directory.is_directory() )
				mCurrentDirectory /= path.filename();

		ImGui::SetWindowFontScale(1);
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
								renderer->GetTextureScaleDeviation()*transform->GetScale().y ) : glm::vec2( 10, 10 ), false ) );
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
	if( ImGui::CollapsingHeader( ICON_FA_IMAGE " Sprite renderer", ImGuiTreeNodeFlags_DefaultOpen ) )
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
			ImGui::Button( ICON_FA_ARROW_DOWN "##DropTexture", ImVec2(64.0f,64.0f) );
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
	ImGui::CollapsingHeader( ICON_FA_SQUARE " Box Collider", ImGuiTreeNodeFlags_DefaultOpen );

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
	ImGui::CollapsingHeader( ICON_FA_FILM " Animation", ImGuiTreeNodeFlags_DefaultOpen );

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

		ImGui::SameLine();
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
				ImGui::Button( ICON_FA_ARROW_DOWN "##DropSprite", ImVec2(size,size) );
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

void CModuleEditor::RenderGizmoPanel()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

	ImGui::Begin( "  " ICON_FA_GRIP_LINES "##GizmoPanel", nullptr, windowFlags );

	ImGui::Dummy( ImVec2( 0, 1 ) );
	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 13.0f );
	if( ImGui::Button( ICON_FA_HAND_ROCK "##GO free move" ) )
	{
		// GO Selection mode
	}

	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 13.0f );
	if( ImGui::Button( ICON_FA_HAND_POINT_RIGHT "##GO move" ) )
	{
		// GO axis drag mode
	}

	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 13.0f );
	if( ImGui::Button( ICON_FA_RULER_COMBINED "##GO scale" ) )
	{
		// GO axis scale mode
	}

	ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 13.0f );
	if( ImGui::Button( ICON_FA_SPINNER "##GO rotate" ) )
	{
		// GO rotation mode
	}

	ImGui::End();
	ImGui::PopStyleVar(3);
}
