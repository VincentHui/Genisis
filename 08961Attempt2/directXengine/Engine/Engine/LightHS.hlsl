
cbuffer TessellationBuffer
{
	float tessellationAmount;
	float3 cameraPosition;
	float3 Misc;
	float LODModifier;
};

struct HullInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
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




////////////////////////////////////////////////////////////////////////////////
// Patch Constant Function
////////////////////////////////////////////////////////////////////////////////
ConstantOutputType ColorPatchConstantFunction(InputPatch<HullInputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{    
	ConstantOutputType output;


	// Set the tessellation factors for the three edges of the triangle.

	if (Misc.x < 1)
	{
		output.edges[0] = tessellationAmount;
		output.edges[1] = tessellationAmount;
		output.edges[2] = tessellationAmount;
		output.inside = tessellationAmount;
	}
	else
	{
		float d = distance(cameraPosition, inputPatch[0].position.xyz);
		float viewAmount = 64/(d * d) * LODModifier;

		output.edges[0] = viewAmount;
		output.edges[1] = viewAmount;
		output.edges[2] = viewAmount;
		output.inside = viewAmount;
	}

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Hull Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ColorPatchConstantFunction")]

HullOutputType LightHullShader(InputPatch<HullInputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HullOutputType output;


	// Set the position for this control point as the output position.
	output.position = patch[pointId].position.xyz;
	output.tex = patch[pointId].tex;
	output.normal = patch[pointId].normal;
	// Set the input color as the output color.
	output.color = patch[pointId].color;

	return output;
}



[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ColorPatchConstantFunction")]

HullOutputType FractionalOdd(InputPatch<HullInputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HullOutputType output;


	// Set the position for this control point as the output position.
	output.position = patch[pointId].position.xyz;
	output.tex = patch[pointId].tex;
	output.normal = patch[pointId].normal;
	// Set the input color as the output color.
	output.color = patch[pointId].color;

	return output;
}

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ColorPatchConstantFunction")]

HullOutputType FractionalEven(InputPatch<HullInputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HullOutputType output;


	// Set the position for this control point as the output position.
	output.position = patch[pointId].position.xyz;
	output.tex = patch[pointId].tex;
	output.normal = patch[pointId].normal;
	// Set the input color as the output color.
	output.color = patch[pointId].color;

	return output;
}
