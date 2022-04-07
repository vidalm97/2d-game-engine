#include "CApplication.h"

#include <iostream>

CApplication* App;

int main()
{
	App = new CApplication();
	if( !App->Start() )
		return 0;

	EAppStatus state = App->Init();

	while( state != EAppStatus::STOP )
	{
		state = App->Update();
	}

	App->Clear();
	std::cout << "Quit Engine..." << std::endl;

	return 0;
}
