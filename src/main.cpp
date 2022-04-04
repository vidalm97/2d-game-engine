#include "CApplication.h"

#include <iostream>

int main()
{

	CApplication app = CApplication();
	if( !app.Start() )
		return 0;

	EAppStatus state = app.Init();

	while( state != EAppStatus::STOP )
	{
		state = app.Update();
	}

	app.Clear();
	std::cout << "Quit Engine..." << std::endl;

	return 0;
}
