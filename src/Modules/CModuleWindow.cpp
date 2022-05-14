#include "Modules/CModuleWindow.h"

#include "CApplication.h"
#include "Modules/CModuleCamera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

CModuleWindow::CModuleWindow( const int aWidth, const int aHeight ) : mWidth(aWidth), mHeight(aHeight)
{
}

void OnResize( GLFWwindow* aWindow, int aWidth, int aHeight )
{
	glViewport( 0, 0, aWidth, aHeight );
	App->mWindow->UpdateDimensions();
	App->mCamera->UpdateProjectionMatrix();
}

void OnScroll( GLFWwindow* aWindow, double aXOffset, double aYOffset )
{
	App->mCamera->Focus( std::copysign( 1.0f, -aYOffset ) );
}

bool CModuleWindow::Init()
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	mWidth = mFullScreen ? glfwGetVideoMode(glfwGetPrimaryMonitor())->width : 640;
	mHeight = mFullScreen ? glfwGetVideoMode(glfwGetPrimaryMonitor())->height : 400;

	mWindow = glfwCreateWindow( mWidth, mHeight, "2d Game Engine", NULL, NULL );
	if ( mWindow == nullptr )
		return false;

	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback( mWindow, OnResize );
	glfwSetScrollCallback( mWindow, OnScroll );

	return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

bool CModuleWindow::PreUpdate()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwPollEvents();

	return true;
}

bool CModuleWindow::Update()
{
	return !glfwWindowShouldClose(mWindow);
}

bool CModuleWindow::PostUpdate()
{
	glfwSwapBuffers(mWindow);

	return true;
}

bool CModuleWindow::Clear()
{
	glfwTerminate();

	return true;
}

void CModuleWindow::UpdateDimensions()
{
	glfwGetFramebufferSize( mWindow, &mWidth, &mHeight );
}

const float CModuleWindow::GetWidth() const
{
	return mWidth;
}

const float CModuleWindow::GetHeight() const
{
	return mHeight;
}
