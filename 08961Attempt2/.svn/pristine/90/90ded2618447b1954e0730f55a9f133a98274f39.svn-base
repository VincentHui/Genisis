//globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TorusBuffer
{
    float innerRadius;
    float outerRadius;
	float2 padding;
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
	float4 wpos : SVPOS;
};



////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("quad")]
PixelInputType TorusDomainShader(ConstantOutputType input, float2 uv : SV_DomainLocation, const OutputPatch<HullOutputType, 4> patch)
{

    PixelInputType output;
 
	//float4 BasisU = BernsteinBasis( uv.x );
 //   float4 BasisV = BernsteinBasis( uv.y );


	const float PI = 3.14159265358979323846;

	float2 NeoUV = uv.xy * 2 * PI;
    // Determine the position of the new vertex.
	float CenterRadius = outerRadius;
	float tubeRadius = innerRadius;

    float x	= (CenterRadius+ tubeRadius * cos(NeoUV.y)) * cos(NeoUV.x);
	float y = (CenterRadius+ tubeRadius *cos(NeoUV.y)) * sin(NeoUV.x);
	float z = tubeRadius * sin(NeoUV.y);
	float3 pos = float3(x, y, z);

	float TubeCenterX	= (CenterRadius+ 0 * cos(NeoUV.y)) * cos(NeoUV.x);
	float TubeCenterY = (CenterRadius+ 0 *cos(NeoUV.y)) * sin(NeoUV.x);
	float TubeCenterZ = 0 * sin(NeoUV.y);
	float3 TubeCenter = float3(TubeCenterX, TubeCenterY, TubeCenterZ);

	float3 Vnormal = pos - TubeCenter;
	
	Vnormal = normalize( mul(float4(Vnormal.xyz, 1.0f), worldMatrix)).xyz;
	output.normal = Vnormal;

	output.position = float4 (pos.xyz, 1.0f);

	output.position = mul( output.position, worldMatrix);
	output.wpos = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix); 

	output.tex = uv;
    return output;
}