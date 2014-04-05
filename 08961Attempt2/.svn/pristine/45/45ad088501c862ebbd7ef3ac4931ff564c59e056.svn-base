//globals
Texture2D shaderTexture;
Texture2D BaseTexture;
SamplerState SampleType;

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TesDisplacement
{
    float bias;
    float scale;
	float2 XY;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding1;
};

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct HullOutputType
{
    float3 position : POSITION;
    float4 color : COLOR;
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 wpos : SVWPOS;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
    float4 color : COLOR;
	float scale : SVSCALE;
};

float3 filterNormal(float2 uv, float texelSize, float texelAspect)
{
	float4 h;
	h[0] = shaderTexture.SampleLevel(SampleType, uv + texelSize*float2( 0,-1), 0).r * texelAspect;
	h[1] = shaderTexture.SampleLevel(SampleType, uv + texelSize*float2(-1, 0), 0).r * texelAspect;
	h[2] = shaderTexture.SampleLevel(SampleType, uv + texelSize*float2( 1, 0), 0).r * texelAspect;
	h[3] = shaderTexture.SampleLevel(SampleType, uv + texelSize*float2( 0, 1), 0).r * texelAspect;
	float3 n;
	n.z = h[0] - h[3];
	n.x = h[1] - h[2];
	n.y = 1;
	return normalize(n);
}


////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]

PixelInputType LightDomainShader(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, 3> patch)
{
    float3 vertexPosition;
    PixelInputType output;
 
	output.tex = uvwCoord.x * patch[0].tex + uvwCoord.y * patch[1].tex + uvwCoord.z * patch[2].tex;;
    // Determine the position of the new vertex.

    vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
   
	// Calculate the normal vector against the world matrix only.
	float3 vNormal = uvwCoord.x * patch[0].normal + uvwCoord.y * patch[1].normal + uvwCoord.z * patch[2].normal;
	output.normal = normalize(vNormal);
	//output.uvw = uvwCoord;

	float Displacement = shaderTexture.SampleLevel(SampleType, output.tex.xy, 0).r;

	Displacement *= scale;
	output.scale = scale;
	float3 vDirection = output.normal;
	
	vertexPosition += vDirection * Displacement;
	
	output.normal = filterNormal(output.tex, 0.01, scale);
    //output.normal = normalize(output.normal);

	output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
	output.wpos = output.position;
	//if(distance(output.wpos, float3(0.0, 0.0, 0.0)) > 10)
	//{
	//	output.wpos = output.position += float4(0, -2, 0, 0);
	//}
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Send the input color into the pixel shader.
    output.color =  BaseTexture.SampleLevel(SampleType, output.tex.xy, 0).r;


    return output;
}