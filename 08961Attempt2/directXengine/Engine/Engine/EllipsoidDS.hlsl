//globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer EllispoidBuffer
{
	float XLen;
	float YLen;
	float ZLen;
	float padding;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct HullOutputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};



////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("quad")]
PixelInputType EllipsoidDomainShader(ConstantOutputType input, float2 uv : SV_DomainLocation, const OutputPatch<HullOutputType, 4> patch)
{

    PixelInputType output;
 
	const float PI = 3.14159265358979323846;

	float NeoU = uv.y * 2 * PI;
	float NeoV = uv.x * PI;

	float x	= XLen * cos(NeoU) * sin(NeoV);	
	float y	= YLen * sin(NeoU) * sin(NeoV);
	float z	= ZLen * cos(NeoV);


	output.position = float4 (x, y, z, 1.0f);
	output.normal = normalize(output.position.xyz);
	output.position = mul( output.position, worldMatrix);


    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix); 
	output.tex = float2(NeoU, NeoV);



    return output;
}