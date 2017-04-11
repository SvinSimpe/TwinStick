#include "GraphicSystem.h"
#include "Utility.h"
#include <DirectXColors.h>
#include "ActorCollection.h"

#pragma comment( lib, "d3dcompiler.lib" )
#include <d3dcompiler.h>
#include <fstream>


using namespace DirectX;
using Microsoft::WRL::ComPtr;

#define GRAPHIC_MASK ( EComponentType::Transform | EComponentType::Mesh )

bool GraphicSystem::InitializeDirectXComponents()
{
	RECT rc;
	GetWindowRect( mWindowHandle, &rc );

	//AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	int border = 12;

	int width	= rc.right - rc.left - border;
	int height	= rc.bottom - rc.top - border;




	// Create Device, Device Context and Swap Chain
	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE };

	DXGI_SWAP_CHAIN_DESC sd;
	memset( &sd, 0, sizeof( sd ) );
	sd.BufferCount				= 1;
	sd.BufferDesc.Width						= Resolution::SCREEN_WIDTH;
	sd.BufferDesc.Height					= Resolution::SCREEN_HEIGHT;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							= mWindowHandle;
	sd.SampleDesc.Count						= 1;
	sd.SampleDesc.Quality					= 0;
	sd.Windowed								= TRUE;
	sd.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	HRESULT hr = E_FAIL;
	for ( UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE( driverTypes ) && FAILED( hr ); driverTypeIndex++ )
	{
		hr = D3D11CreateDeviceAndSwapChain( nullptr,
											driverTypes[driverTypeIndex],
											NULL,
											D3D11_CREATE_DEVICE_DEBUG,
											featureLevelsToTry,
											ARRAYSIZE( featureLevelsToTry ),
											D3D11_SDK_VERSION,
											&sd,
											&mSwapChain,
											&mDevice,
											&initiatedFeatureLevel,
											&mDeviceContext );
	}

	if( FAILED( hr ) )
		return hr;

	// Create Render Target View
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	if ( SUCCEEDED( mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer ) ) )
	{
		hr = mDevice->CreateRenderTargetView( backBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf() );

	}

	if( FAILED( hr ) )
		return false;


	// Create Depth Stencil View
	ComPtr<ID3D11Texture2D> depthStencil = nullptr;

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width				= Resolution::SCREEN_WIDTH;
	dsd.Height				= Resolution::SCREEN_HEIGHT;
	dsd.MipLevels			= 1;
	dsd.ArraySize			= 1;
	dsd.Format				= DXGI_FORMAT_D32_FLOAT;
	dsd.SampleDesc.Count	= 1;
	dsd.SampleDesc.Quality	= 0;
	dsd.Usage				= D3D11_USAGE_DEFAULT;
	dsd.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags		= 0;
	dsd.MiscFlags			= 0;

	if( FAILED( mDevice->CreateTexture2D( &dsd, nullptr, depthStencil.GetAddressOf() ) ) )
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof( dsvd ) );
	dsvd.Format				= dsd.Format;
	dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvd.Texture2D.MipSlice	= 0;

	if( FAILED( mDevice->CreateDepthStencilView( depthStencil.Get(), &dsvd, mDepthStencilView.GetAddressOf() ) ) )
		return hr;

	SetViewport();

	// Create Rasterizer State
	D3D11_RASTERIZER_DESC desc;
	memset( &desc, 0, sizeof( desc ) );
	desc.FillMode			= D3D11_FILL_SOLID;
	desc.CullMode			= D3D11_CULL_BACK;
	desc.DepthClipEnable	= true;

	if( FAILED( mDevice->CreateRasterizerState( &desc, mRasterizerState.GetAddressOf() ) ) )
		return false;

	return true;
}

bool GraphicSystem::InitializeShaders()
{
	// Compile Vertex Shader
	ComPtr<ID3DBlob> vs = nullptr;
	if ( CompileShader( "Shader.hlsl", "VertexShaderMain", "vs_5_0", nullptr, vs.GetAddressOf() ) )
	{
		HRESULT hr = S_OK;
		if( SUCCEEDED( hr = mDevice->CreateVertexShader( vs->GetBufferPointer(),
														 vs->GetBufferSize(),
														nullptr,
														mVertexShader.GetAddressOf() ) ) )
		{	
			D3D11_INPUT_ELEMENT_DESC inputDesc[] = {				 
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },

				{ "WORLD",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD",	  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD",	  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD",	  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 } };

			hr = mDevice->CreateInputLayout( inputDesc,
											ARRAYSIZE( inputDesc ),
											vs->GetBufferPointer(),
											vs->GetBufferSize(),
											mInputLayout.GetAddressOf() );

		}

		// Compile Pixel Shader
		ComPtr<ID3DBlob> ps = nullptr;

		if( CompileShader( "Shader.hlsl", "PixelShaderMain", "ps_5_0", nullptr, ps.GetAddressOf() ) )
		{
			hr = mDevice->CreatePixelShader( ps->GetBufferPointer(),
											ps->GetBufferSize(),
											nullptr,
											mPixelShader.GetAddressOf() );

		}	
		else if( hr == E_FAIL )
			return false;
	}

	return true;
}

bool GraphicSystem::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
{
	DWORD dwShaderFlags =	D3DCOMPILE_ENABLE_STRICTNESS | 
		D3DCOMPILE_IEEE_STRICTNESS | D3DCOMPILE_DEBUG;

	std::string shader_code;
	std::ifstream in( shaderFile, std::ios::in | std::ios::binary );

	if ( in )
	{
		in.seekg( 0, std::ios::end );
		shader_code.resize( (unsigned int)in.tellg() );
		in.seekg( 0, std::ios::beg );
		in.read( &shader_code[0], shader_code.size() );
		in.close();
	}

	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3DCompile( shader_code.data(),
					 shader_code.size(),
					 NULL,
					 pDefines,
					 nullptr,
					 pEntrypoint,
					 pTarget,
					 dwShaderFlags,
					 NULL,
					 pCompiledShader,
					 &errorBlob );

	if( errorBlob )
		OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );

	return true;

}

void GraphicSystem::BeginFrame()
{
	SetViewport();
	mDeviceContext->ClearDepthStencilView( mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
	mDeviceContext->ClearRenderTargetView( mRenderTargetView.Get(), Colors::Black );
	mDeviceContext->OMSetRenderTargets( 1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get() );
	

	// Set VertexData buffer and InstanceData buffer
	UINT32 stride[2]				= { sizeof(Vertex32), sizeof(InstanceData) };
	UINT32 offset[2]				= { 0, 0 };
	ID3D11Buffer* buffersToSet[2]	= { mBuffers[static_cast<size_t>(EBufferType::Vertex)].Get() ,
										mBuffers[static_cast<size_t>( EBufferType::Instance )].Get() };
	mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, stride, offset );
	mDeviceContext->VSSetConstantBuffers( 0, 1, mBuffers[static_cast<size_t>( EBufferType::Frame )].GetAddressOf() );
	





	mDeviceContext->RSSetState( mRasterizerState.Get() );
	mDeviceContext->IASetInputLayout( mInputLayout.Get() );
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );



	mDeviceContext->VSSetShader( mVertexShader.Get(), nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mPixelShader.Get(), nullptr, 0 );
	
}

void GraphicSystem::EndFrame()
{
	// Swap Front and Back Buffer
	mSwapChain->Present( 1, 0 );
	mDeviceContext->ClearState();

}

bool GraphicSystem::Render( const size_t numActiveActors )
{
	BeginFrame();

	// Render Components;
	mDeviceContext->DrawInstanced( static_cast<UINT>( mCubeMesh->vertices.size() ),
								   static_cast<UINT>(numActiveActors), 0, 0 );

	EndFrame();
	return true;
	
}

void GraphicSystem::SetViewport()
{
	RECT rc;
	GetClientRect( mWindowHandle, &rc );

	int width	= ( rc.right - rc.left );
	int height	= ( rc.bottom - rc.top );

	D3D11_VIEWPORT vp;
	vp.Width	= Resolution::SCREEN_WIDTH;
	vp.Height	= Resolution::SCREEN_HEIGHT;
	vp.MinDepth	= 0.0f;
	vp.MaxDepth	= 1.0f;
	vp.TopLeftX	= 0;
	vp.TopLeftY	= 0;

	mDeviceContext->RSSetViewports( 1, &vp );
	mDeviceContext->OMSetRenderTargets( 1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get() );

}

bool GraphicSystem::BuildMeshVBuffer()
{
	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth			= sizeof(Vertex32) * static_cast<UINT>( mCubeMesh->vertices.size() );
	vbd.StructureByteStride = sizeof(Vertex32);
	vbd.Usage				= D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags		= 0;
	vbd.MiscFlags			= 0;


	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.SysMemPitch		= 0;
	vinitData.SysMemSlicePitch	= 0;
	vinitData.pSysMem			= &mCubeMesh->vertices[0];

	if( FAILED( mDevice->CreateBuffer( &vbd, &vinitData,
									   mBuffers[static_cast<size_t>(EBufferType::Vertex)].GetAddressOf() ) ) )
		return false;

	return true;
}

bool GraphicSystem::BuildFrameCBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth			= sizeof( FrameData );
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags			= 0;
	cbDesc.StructureByteStride	= 0;

	if( FAILED ( mDevice->CreateBuffer( &cbDesc, nullptr,
										mBuffers[static_cast<size_t>(EBufferType::Frame)].GetAddressOf() ) ) )
		return false;

	return true;
}

bool GraphicSystem::BuildInstanceBuffer()
{
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth			= sizeof( InstanceData ) * GameGlobals::MAX_ACTORS;
	ibDesc.Usage				= D3D11_USAGE_DYNAMIC;
	ibDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	ibDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	ibDesc.MiscFlags			= 0;
	ibDesc.StructureByteStride	= 0;

	if( FAILED( mDevice->CreateBuffer( &ibDesc, nullptr,
									   mBuffers[static_cast<size_t>(EBufferType::Instance)].GetAddressOf() ) ) )
		return false;

	return true;
}

bool GraphicSystem::UpdateFrameCBuffer( FrameData &newFrameData )
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = mDeviceContext->Map( mBuffers[static_cast<size_t>(EBufferType::Frame)].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &newFrameData, sizeof(FrameData) );
		mDeviceContext->Unmap( mBuffers[static_cast<size_t>(EBufferType::Frame)].Get(), 0 );
	}
	else
		return false;

	return true;

}

bool GraphicSystem::UpdateInstanceCBuffer( std::unique_ptr<ActorCollection>& actors, const size_t numActiveActor )
{
	if( actors )
	{
		std::vector<InstanceData> actorWorldMatrices;

		for( size_t i = 0; i < numActiveActor; i++ )
		{
			if( actors->mIsActive[i] &&
				( actors->componentMasks[i] & GRAPHIC_MASK ) == GRAPHIC_MASK )
			{ 			
				TransformComponent& actorTransform = *actors->mTransformComponents[i];

				XMMATRIX scale			= XMMatrixScalingFromVector( XMLoadFloat3( &actorTransform.scale ) );
				XMMATRIX rotation		= XMMatrixRotationY( actorTransform.rotation.y );
				XMMATRIX translation	= XMMatrixTranslationFromVector( XMLoadFloat3( &actorTransform.location ) );
				XMMATRIX newWorldMatrix	= scale* rotation * translation;

				// Store new World matrix and Color
				InstanceData actorWorldAndColor;				
				XMStoreFloat4x4( &actorWorldAndColor.worldMatrix, newWorldMatrix );
				actorWorldAndColor.color = actors->mMeshComponents[i]->color;
				actorWorldMatrices.push_back( actorWorldAndColor );

			}
		}

		if( !actorWorldMatrices.empty() )
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT hr = mDeviceContext->Map( mBuffers[static_cast<size_t>(EBufferType::Instance)].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

			
			if( SUCCEEDED( hr ) )
			{

				memcpy( mappedResource.pData, &actorWorldMatrices[0], sizeof(InstanceData) * actorWorldMatrices.size() );
				mDeviceContext->Unmap( mBuffers[static_cast<size_t>(EBufferType::Instance)].Get(), 0 ); 
			}
			else
				return false;

		}
	}

	return true;

}

GraphicSystem::GraphicSystem()
{
	mWindowHandle		= NULL;

	mDevice				= nullptr;
	mDeviceContext		= nullptr;
	mSwapChain			= nullptr;
	mRenderTargetView	= nullptr;
	mDepthStencilView	= nullptr;
	mRasterizerState	= nullptr;
	mSamplerState		= nullptr;

	for( size_t i = 0; i < static_cast<size_t>(EBufferType::NumBufferTypes); i++ )
		mBuffers.push_back( nullptr );

	mCubeMesh			= nullptr;
}

GraphicSystem::~GraphicSystem()
{}

bool GraphicSystem::Initialize( HWND& windowHandle )
{
	mWindowHandle = windowHandle;

	if( !InitializeDirectXComponents() )
		return false;

	if( !InitializeShaders() )
		return false;

	mCubeMesh = std::make_unique<CubeMesh>();

	if( !BuildMeshVBuffer() )
		return false;

	if( !BuildFrameCBuffer() )
		return false;

	if( !BuildInstanceBuffer() )
		return false;

	


	return true;

}

bool GraphicSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							size_t numActiveActors, void* systemSpecificInput )
{
	if( !actors )
		return false;

	if( !UpdateFrameCBuffer( *( static_cast<FrameData*>( systemSpecificInput ) ) ) )
		return false;
	
	if( !UpdateInstanceCBuffer( actors, numActiveActors ) )
		return false;
	
	if( !Render( numActiveActors ) )
		return false;

	

	return true;

}
