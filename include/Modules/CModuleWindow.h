#ifndef CMODULEWINDOW__H
#define CMODULEWINDOW__H

#include "AModule.h"

class GLFWwindow;

class CModuleWindow : public AModule
{
public:
	CModuleWindow( const int aWidth, const int aHeight );
	~CModuleWindow() = default;

	bool Init() override;

	bool PreUpdate() override;
	bool Update() override;
	bool PostUpdate() override;

	bool Clear() override;

	const int GetWidth() const;
	const int GetHeight() const;

	GLFWwindow *mWindow = nullptr;

private:
	int mWidth, mHeight;
};

#endif
