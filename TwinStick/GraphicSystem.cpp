#include "GraphicSystem.h"
#include "Utility.h"
#include <DirectXColors.h>

using namespace DirectX;;
using Microsoft::WRL::ComPtr;

void GraphicSystem::BeginFrame()
{
	mDeviceContext->ClearDepthStencilView( mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
	mDeviceContext->ClearRenderTargetView( mRenderTargetView.Get(), Colors::PaleVioletRed );
	mDeviceContext->OMSetRenderTargets( 1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get() );

}

void GraphicSystem::EndFrame()
{
	// Swap Front and Back Buffer
	mSwapChain->Present( 0, 0 );
	mDeviceContext->ClearState();

}

bool GraphicSystem::Render()
{
	BeginFrame();

	// Render Components;

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

}

GraphicSystem::GraphicSystem( HWND& windowHandle )
{
	mWindowHandle = windowHandle;


}

GraphicSystem::~GraphicSystem()
{}

bool GraphicSystem::Initialize( HWND& windowHandle )
{
	mWindowHandle = windowHandle;

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
	//ID3D11Texture2D* pBackBuffer;
	if ( SUCCEEDED( hr = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&backBuffer ) ) )
	{
		hr = mDevice->CreateRenderTargetView( backBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf() );
		//SAFE_RELEASE( backBuffer );
	}

	if( FAILED( hr ) )
		return hr;


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

	if( FAILED( hr = mDevice->CreateTexture2D( &dsd, nullptr, depthStencil.GetAddressOf() ) ) )
		return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof( dsvd ) );
	dsvd.Format				= dsd.Format;
	dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvd.Texture2D.MipSlice	= 0;

	if( FAILED( hr = mDevice->CreateDepthStencilView( depthStencil.Get(), &dsvd, mDepthStencilView.GetAddressOf() ) ) )
		return hr;

	SetViewport();

	return true;

}


bool GraphicSystem::Update( float deltaTime, std::unique_ptr<ActorCollection>& actors )
{

	if( !Render() )
		return false;

	return true;
}
