#include "Engine.h"
#include <memory>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{

	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	if( !engine->Initialize( hInstance, nCmdShow ) )
		return 0;
	for( size_t i = 0; i < 10; i++ )
	{
		std::vector<std::unique_ptr<IComponent>> componentList;
		std::unique_ptr<TransformComponent> transform = std::make_unique<TransformComponent>( XMFLOAT3( i * 2.0f, 0.0f, 0.0f ) );
		std::unique_ptr<MeshComponent> mesh = std::make_unique<MeshComponent>();

		componentList.push_back( std::move( transform ) );
		componentList.push_back( std::move( mesh ) );

		engine->RequestActor( componentList );
	}
	

	return engine->Run();

}