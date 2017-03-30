cbuffer CB_PER_FRAME : register(b0)
{
	column_major float4x4  view;
	column_major float4x4  proj;
	float4	  eyePosition;
};

struct VertexInput
{
	// Vertex data
	float3				position	: POSITION;
	float3				normal		: NORMAL;
	float2				texCoord	: TEXCOORD;

	// Instance data
	row_major float4x4	world		: WORLD;
	float4				color		: COLOR;
	uint				instanceID	: SV_InstanceID;
};

struct PixelInput
{
	float4 position	: SV_POSITION;
	float3 normal	: NORMAL;
	float2 texCoord	: TEXCOORD;
	float4 color	: COLOR;
};





//---------------------------------------------------------------------------------------------------------------------
// Vertex Shader
//---------------------------------------------------------------------------------------------------------------------
PixelInput VertexShaderMain( VertexInput vertexInput )
{
	PixelInput pixelInput = (PixelInput)0;


	pixelInput.position		= mul( float4( vertexInput.position, 1.0f ), vertexInput.world );
	pixelInput.position		= mul( pixelInput.position, view );
	pixelInput.position		= mul( pixelInput.position, proj );

	pixelInput.normal		= normalize( mul( vertexInput.normal, (float3x3)vertexInput.world ) );
	pixelInput.texCoord		= vertexInput.texCoord;
	pixelInput.color		= vertexInput.color;

	return pixelInput;
}


Texture2D		inputTexture : register(t0);
SamplerState	samplerState : register(s0);


//---------------------------------------------------------------------------------------------------------------------
// Pixel Shader
//---------------------------------------------------------------------------------------------------------------------
float4 PixelShaderMain( PixelInput pixelInput ) : SV_Target
{
	return pixelInput.color;

	//return inputTexture.Sample( samplerState, pixelInput.texCoord );

}