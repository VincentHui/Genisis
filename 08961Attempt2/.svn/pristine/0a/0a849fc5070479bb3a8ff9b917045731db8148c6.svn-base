#define INPUT_PATCH_SIZE 16

cbuffer TessellationBuffer
{
    float tessellationAmount;
    float3 cameraPosition;
	float4 Misc;
};

struct HullInputType
{
    float4 position : POSITION;
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct HullOutputType
{
    float3 position : POSITION;
    //float4 color : COLOR;
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};



////////////////////////////////////////////////////////////////////////////////
// Patch Constant Function
////////////////////////////////////////////////////////////////////////////////
ConstantOutputType ColorPatchConstantFunction(InputPatch<HullInputType, INPUT_PATCH_SIZE> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;

	float d = distance(cameraPosition, inputPatch[0].position.xyz);

    // Set the tessellation factors for the three edges of the triangle.

	Misc.x;
	float viewAmount = 70/(d);
    output.edges[0] = tessellationAmount;
    output.edges[1] = tessellationAmount;
    output.edges[2] = tessellationAmount;
	output.edges[3] = tessellationAmount;

    // Set the tessellation factor for tessallating inside the triangle.
    output.inside[0] = tessellationAmount;
	output.inside[1] = tessellationAmount;

    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("ColorPatchConstantFunction")]

HullOutputType BezierHullShader(InputPatch<HullInputType, 16> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HullOutputType output;


    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;
	output.tex = patch[pointId].tex;
	output.normal = patch[pointId].normal;
    return output;
}

[domain("quad")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("ColorPatchConstantFunction")]

HullOutputType FractionalOdd(InputPatch<HullInputType, 16> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HullOutputType output;


    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;
	output.tex = patch[pointId].tex;
	output.normal = patch[pointId].normal;
    return output;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(16)]
[patchconstantfunc("ColorPatchConstantFunction")]

HullOutputType FractionalEven(InputPatch<HullInputType, 16> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HullOutputType output;


    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;
	output.tex = patch[pointId].tex;
	output.normal = patch[pointId].normal;
    return output;
}