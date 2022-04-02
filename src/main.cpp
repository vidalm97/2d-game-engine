#include "CApplication.h"

#include <iostream>

int main()
{

	CApplication app = CApplication();
	EAppStatus state = app.Init();

	while( state != EAppStatus::STOP )
	{
		std::cout << "Update Engine..." << std::endl;
		state = app.Update();
	}

	app.Clear();
	std::cout << "Quit Engine..." << std::endl;

	return 0;
}
