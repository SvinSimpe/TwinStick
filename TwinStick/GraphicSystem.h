//#pragma once



#include <wrl.h>
#include "stdafx.h"

#include "ISystems.h"
#include "CubeMesh.h"

#include "EBufferType.h"
#include "CBufferTypes.h"

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
		
		Microsoft::WRL::ComPtr<ID3D11VertexShader>		mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>		mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>		mInputLayout;

		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> mBuffers;


		std::unique_ptr<CubeMesh>	mCubeMesh;

	private:
		bool CreateRenderTargetView();
		bool CreateDepthStencilView();
		bool CreateRasterizerState();
		bool InitializeDirectXComponents();
		bool InitializeShaders();
		bool CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget,
							D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );

		void BeginFrame();
		void EndFrame();
		void Render( const size_t numActiveActors );
		void SetViewport();

		bool BuildMeshVBuffer();
		bool BuildFrameCBuffer();
		bool BuildInstanceBuffer();

		bool UpdateFrameCBuffer( FrameData& newFrameData );
		bool UpdateInstanceCBuffer( std::unique_ptr<ActorCollection>& actors,
									const size_t numActiveActors );

	public:
		GraphicSystem();
		~GraphicSystem();
		bool Initialize( HWND& windowHandle ) ;
		

		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							 size_t numActiveActors, void* systemSpecificInput ) override;
};
