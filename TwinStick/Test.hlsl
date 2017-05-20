cbuffer CB_PER_FRAME : register( b0 )
{
	column_major float4x4 view;
	column_major float4x4 proj;
	float4 eyePosition;

};

struct VertexInput
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 texCoord : TEXCOORD;

	row_major float4x4 world : WORLD;
	float4 color : COLOR;
	uint instanceID : SV_InstanceID;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
	float2 texCoord : TEXCOORD;
	float4 color	: COLOR;

};

PixelInput VertexShaderMain( VertexInput input )
{

	PixelInput p = (PixelInput)0;

	return p;
}

float4 PixelShaderMain( PixelInput input ) : SV_Target
{

	return float4( 1.0f, 1.0f, 0.0f, 1.0f );
}

