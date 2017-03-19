#include "Engine.h"
#include <memory>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	if( !engine->Initialize( hInstance, nCmdShow ) )
		return 0;

	return engine->Run();

}