#include "Modules/CModuleInput.h"

#include "CApplication.h"
#include "Modules/CModuleCamera.h"
#include "Modules/CModuleWindow.h"

#include "GLFW/glfw3.h"

bool CModuleInput::HandleInput()
{
	if ( glfwGetKey( App->mWindow->mWindow, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		return false;

	if ( glfwGetKey( App->mWindow->mWindow, GLFW_KEY_D ) == GLFW_PRESS )
		App->mCamera->MoveCamera( glm::vec3( 1.0f, 0.0f, 0.0f ) );

	if ( glfwGetKey( App->mWindow->mWindow, GLFW_KEY_A ) == GLFW_PRESS )
		App->mCamera->MoveCamera( glm::vec3( -1.0f, 0.0f, 0.0f ) );

	if ( glfwGetKey( App->mWindow->mWindow, GLFW_KEY_W ) == GLFW_PRESS )
		App->mCamera->MoveCamera( glm::vec3( 0.0f, 1.0f, 0.0f ) );

	if ( glfwGetKey( App->mWindow->mWindow, GLFW_KEY_S ) == GLFW_PRESS )
		App->mCamera->MoveCamera( glm::vec3( 0.0f, -1.0f, 0.0f ) );

	return true;
}

bool CModuleInput::Update()
{
	return HandleInput();
}
