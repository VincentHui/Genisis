//globals
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
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


PixelInputType ScreenSpaceVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
	//output.position = float4(output.tex, 0.0, 1.0); 
    // Calculate the position of the vertex against the world, view, and projection matrices.
    //output.position = mul(input.position, worldMatrix);
    //output.position = mul(input.position, viewMatrix);
    //output.position = mul(output.position, projectionMatrix);


	output.tex = sign(input.position.xy);
	output.position = float4(output.tex, 0.0, 1.0);
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    
    return output;
}