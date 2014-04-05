cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer particle
{
	float size;
	float time;
	float incrementMod;
	float heightToKill;
};

struct GSOutput
{
	float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	uint VertId : SVSCALE;
};

struct GeoInput
{
	float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	uint VertId : SVVERTID;
};

void ScreenAlignedQuad(GSOutput output, float tempSize, float4 tempPos, inout TriangleStream< GSOutput > newTriangles)
{
	//topleft
	output.position = tempPos + float4(-tempSize, tempSize, 0 ,0);
	output.tex = float2(0, 1);
	newTriangles.Append(output);


	//topright
	output.position = tempPos + float4(tempSize, tempSize, 0 ,0);
	output.tex = float2(1, 1);
	newTriangles.Append(output);

	
	//bottom right
	output.position = tempPos + float4(tempSize, -tempSize, 0 ,0) ;	
	output.tex = float2(1, 0);
	newTriangles.Append(output);
	newTriangles.RestartStrip(); 

	//topleft
	output.position = tempPos + float4(-tempSize, tempSize, 0 ,0);
	output.tex = float2(0, 1);
	newTriangles.Append(output);

	//bottom right
	output.position = tempPos + float4(tempSize, -tempSize, 0 ,0) ;
	output.tex = float2(1, 0);
	newTriangles.Append(output);

	//bottom left
	output.position = tempPos + float4(-tempSize, -tempSize, 0 ,0) ;
	output.tex = float2(0, 0);
	newTriangles.Append(output);
	newTriangles.RestartStrip();
}

[maxvertexcount(12)]
void GeoSmokeGeometryShader(
	triangle GeoInput input[3], 
	inout TriangleStream< GSOutput > newTriangles)
{
	GSOutput output;
	float pixelSize;
	
	////sine increment
	float sineIncrement = sin(time + input[0].VertId) * incrementMod;
	float tempIncrement = time * incrementMod;
	float particleDiff = input[0].VertId;
	////random

	float4 tempPos = input[0].position + float4( 0,  (particleDiff * 0.001)* time %2 ,0, 0);


	float tempSize = size * sineIncrement;

	if(sineIncrement < 0)
	{
		tempSize = 0;
	}

	tempPos = mul(tempPos, worldMatrix);
	tempPos = mul(tempPos, viewMatrix);
	tempPos = mul(tempPos, projectionMatrix);

	output.VertId = input[0].VertId;


	ScreenAlignedQuad(output, tempSize, tempPos, newTriangles);
}