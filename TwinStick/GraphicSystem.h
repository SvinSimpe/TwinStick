//#pragma once



#include <wrl.h>
//#include "stdafx.h"

#include "InputDescriptions.h"
#include "ISystems.h"
#include "CubeMesh.h"
#include "EShaderType.h"
#include "EBufferType.h"
#include "EInputType.h"
#include "CBufferTypes.h"
#include <vector>

struct EffectDesc
{
	char*		shaderFile	= nullptr;
	EShaderType	shaderType	= EShaderType::None;
	EInputType	inputType	= EInputType::None;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader	= nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader		= nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout		= nullptr;// ????

};

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
		
		std::vector<EffectDesc>							mEffects;
		EffectDesc*										mCurrentEffect;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>		mInputLayout;

		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> mBuffers;


		std::unique_ptr<CubeMesh>	mCubeMesh;
		std::vector<XMFLOAT2>		mQuadTreeVertices;

	private:
		bool CreateRenderTargetView();
		bool CreateDepthStencilView();
		bool CreateRasterizerState();
		bool InitializeDirectXComponents();
		bool InitializeEffects();
		bool InitializeShaders( EffectDesc& effect );
		bool CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget,
							D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );

		void BeginFrame();
		void EndFrame();
		void Render( const size_t numActiveActors );
		void SetViewport();

		void RenderInstanced( const size_t numActiveActors );
		void RenderDebug();

		bool BuildMeshVBuffer();
		bool BuildFrameCBuffer();
		bool BuildInstanceBuffer();
		bool BuildQuadTreeDebugVBuffer();

		bool UpdateFrameCBuffer( FrameData& newFrameData );
		bool UpdateInstanceCBuffer( std::unique_ptr<ActorCollection>& actors,
									const size_t numActiveActors );
		bool UpdateQuadTreeVBuffer();

	public:
		GraphicSystem();
		~GraphicSystem();
		bool Initialize( HWND& windowHandle ) ;
		
		void SetQuadTreeVertices( std::vector<XMFLOAT2>& vertices );


		// Inherited via ISystems
		virtual bool Update( float deltaTime, std::unique_ptr<ActorCollection>& actors,
							 size_t numActiveActors, void* systemSpecificInput ) override;
};

