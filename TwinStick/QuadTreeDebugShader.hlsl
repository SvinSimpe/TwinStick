cbuffer CB_PER_FRAME : register(b0)
{
	column_major float4x4 view;
	column_major float4x4 proj;
	float4 eyePosition;
};

struct VertexInput
{
	float3 position : POSITION;
};

struct PixelInput
{
	float4 position : SV_POSITION;
};


PixelInput VertexShaderMain( VertexInput vertexInput )
{
	PixelInput pixelInput = (PixelInput)0;

	row_major float4x4 world = { 1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f };

	pixelInput.position = mul( float4( vertexInput.position, 1.0f ), world );
	pixelInput.position	= mul( pixelInput.position, view );
	pixelInput.position	= mul( pixelInput.position, proj );

	return pixelInput;

}

float4 PixelShaderMain( PixelInput pixelInput ) : Sv_Target
{
	return float4( 0.2f, 1.0f, 1.0f, 1.0f );

}