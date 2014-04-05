////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
////////////////////////////////////////////////////////////////////////////////


cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding1;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct HullInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
HullInputType LightVertexShader(VertexInputType input)
{
    HullInputType output;
    
    output.position = input.position;
    output.color = float4(0.0f, 1.0f, 1.0f, 1.0f);
	output.tex = input.tex;
	output.normal = input.normal;

    return output;
}