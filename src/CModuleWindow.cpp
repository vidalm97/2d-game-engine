#include "CModuleWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

CModuleWindow::CModuleWindow( const int aWidth, const int aHeight ) : mWidth(aWidth), mHeight(aHeight)
{
}

bool CModuleWindow::Init()
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	mWindow = glfwCreateWindow( mWidth, mHeight, "2d Game Engine", NULL, NULL );
	if ( mWindow == nullptr )
		return false;

	glfwMakeContextCurrent(mWindow);

	return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

bool CModuleWindow::Update()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(mWindow);

	return true;
}

bool CModuleWindow::Clear()
{
	glfwTerminate();
	return true;
}
