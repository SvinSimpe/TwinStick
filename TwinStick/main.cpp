#include "Engine.h"
#include <memory>
#include <ctime>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	srand( static_cast<unsigned int>( time( NULL ) ) );

	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	if( !engine->Initialize( hInstance, nCmdShow ) )
		return 0;


	for( size_t i = 0; i < 30; i++ )
	{
		std::vector<std::unique_ptr<IComponent>> componentList;
		std::unique_ptr<TransformComponent> transform = std::make_unique<TransformComponent>();
		std::unique_ptr<MeshComponent> mesh = std::make_unique<MeshComponent>();
		std::unique_ptr<HealthComponent> health = std::make_unique<HealthComponent>( 50.0f, 150.0f );
		std::unique_ptr<MovementComponent> movement = std::make_unique<MovementComponent>();
		std::unique_ptr<SteeringBehaviourComponent> steeringBehaviour = std::make_unique<SteeringBehaviourComponent>();

		movement->targetLocation = XMFLOAT3( RandomFloatInRange( -200.0f, 200.0f ), 0.0f, RandomFloatInRange( -200.0f, 200.0f ) );
		movement->velocity = XMFLOAT3( 0.0f, 0.0f, 1.0f );
		
		
		switch( i % 5 )
		{
			case 0:
			{
				mesh->color = XMFLOAT4( 57.0f/255.0f, 90.0f/255.0f, 79.0f/255.0f, 1.0f );
				movement->speed = 60.0f;
				transform->scale = XMFLOAT3( 2.0f, 4.0f, 2.0f );
				break;
			}

			case 1:
			{
				mesh->color = XMFLOAT4( 67.0f/255.0f, 35.0f/255.0f, 48.0f/255.0f, 1.0f );
				movement->speed = 40.0f;		
				transform->scale = XMFLOAT3( 3.0f, 5.0f, 3.0f );
				break;
			}
			case 2:
			{
				mesh->color = XMFLOAT4( 133.0f/255.0f, 60.0f/255.0f, 67.0f/255.0f, 1.0f );
				movement->speed = 35.0f;
				transform->scale =XMFLOAT3( 4.0f, 6.0f, 4.0f );
				break;
			}
			case 3:
			{
				mesh->color = XMFLOAT4( 242.0f/255.0f, 92.0f/255.0f, 94.0f/255.0f, 1.0f );
				movement->speed = 30.0f;
				transform->scale = XMFLOAT3( 5.0f, 7.0f, 5.0f );
				break;
			}
			case 4:
			{
				mesh->color = XMFLOAT4( 255.0f/255.0f, 165.0f/255.0f, 102.0f/255.0f, 1.0f );
				movement->speed = 20.0f;
				transform->scale = XMFLOAT3( 6.0f, 8.0f, 6.0f );
				break;
			}
			default:
				break;
		}
		
		steeringBehaviour->mass = XMVectorGetX( XMVector3Length( XMLoadFloat3( &transform->scale ) ) );

		componentList.push_back( std::move( transform ) );
		componentList.push_back( std::move( mesh ) );
		componentList.push_back( std::move( health ) );
		componentList.push_back( std::move( movement ) );
		componentList.push_back( std::move( steeringBehaviour ) );
	
		engine->RequestActor( componentList );
	}
		
	

	return engine->Run();

}