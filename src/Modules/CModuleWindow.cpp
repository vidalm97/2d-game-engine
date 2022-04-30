#include "Modules/CModuleWindow.h"

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

	mWidth = mFullScreen ? glfwGetVideoMode(glfwGetPrimaryMonitor())->width : 960;
	mHeight = mFullScreen ? glfwGetVideoMode(glfwGetPrimaryMonitor())->height : 600;

	mWindow = glfwCreateWindow( mWidth, mHeight, "2d Game Engine", NULL, NULL );
	if ( mWindow == nullptr )
		return false;

	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1);

	return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

bool CModuleWindow::PreUpdate()
{
	glfwPollEvents();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	return true;
}

bool CModuleWindow::Update()
{
	return !glfwWindowShouldClose(mWindow);
}

bool CModuleWindow::PostUpdate()
{
	int display_w, display_h;
	glfwGetFramebufferSize( mWindow, &display_w, &display_h );
	glViewport( 0, 0, display_w, display_h );

	glfwSwapBuffers(mWindow);

	return true;
}

bool CModuleWindow::Clear()
{
	glfwTerminate();

	return true;
}

const int CModuleWindow::GetWidth() const
{
	return mWidth;
}

const int CModuleWindow::GetHeight() const
{
	return mHeight;
}
