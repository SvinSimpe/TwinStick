//#pragma once


#include "ISystems.h"
#include <wrl.h>
#include "stdafx.h"


class GraphicSystem : public ISystems
{
	private:
		HWND	mWindowHandle;

		Microsoft::WRL::ComPtr<ID3D11Device>			mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	mRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>		mSamplerState;


	private:
		void BeginFrame();
		void EndFrame();
		bool Render();
		void SetViewport();

	public:
		GraphicSystem();
		~GraphicSystem();
		bool Initialize( HWND& windowHandle ) ;
		

		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors, size_t numActiveActor ) override;
};
