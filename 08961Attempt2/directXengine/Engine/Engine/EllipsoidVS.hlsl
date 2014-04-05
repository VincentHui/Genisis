
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct HullInputType
{
    float4 position : POSITION;
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


HullInputType EllipsoidVertexShader(VertexInputType input)
{
    HullInputType output;
    
    output.position = input.position;
	output.tex = input.tex;
	output.normal = input.normal;

    return output;
}