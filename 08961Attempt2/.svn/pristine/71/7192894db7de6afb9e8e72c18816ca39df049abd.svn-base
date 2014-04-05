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

struct GeoInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

GeoInput GeoALienVertexShader( VertexInputType input )
{
	GeoInput output;

    float4 worldPosition;
	input.position.w = 1.0f;
	//float increment = id * 0.2f;
	//output.position = float4(0.0, 0.0, 0.0, 1.0);
	output.position = input.position;

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

	output.tex = input.tex;
	output.normal = input.normal;
	return output;
}