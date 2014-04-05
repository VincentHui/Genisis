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
	float incrementSizeMod;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 wpos : SVWPOS;
	float3 normal : NORMAL;
};

struct GeoInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


[maxvertexcount(24)]
void GeoAlienGeometryShader(
	triangle GeoInput input[3], 
	inout TriangleStream< GSOutput > newTriangles
	)
{
	
	float2 MediumTex = input[0].tex + input[1].tex + input[2].tex;
	//float3 MediumNormal = input[0].normal + input[1].normal + input[2].normal;
	float4 avg= input[0].position+input[1].position+input[2].position;
	avg.xyz *= (sin(time * 0.5f) + 2.0f)/ 5;
	avg.w=1;
	float3 MediumNormal = normalize(avg.xyz);

	GSOutput output; 
	float4x4 worldViewProj = worldMatrix * viewMatrix * projectionMatrix;
	for (int i=0; i<3; i++) 
	{ 
		//float4 tempOrigin, tempP2, tempP3;
		//tempOrigin = mul(input[i].position, worldMatrix);
		////calculate normal
		//if(i==2)
		//{
		//	tempP2 = mul(input[0].position, worldMatrix);
		//}
		//else
		//{
		//	tempP2 = mul(input[i + 1].position, worldMatrix);
		//}
		//tempP3 = mul(avg,worldMatrix); 


		float3 tempOrigin, tempP2, tempP3;
		tempOrigin =input[i].normal;
		//calculate normal
		if(i==2)
		{
			tempP2 = input[0].normal;
		}
		else
		{
			tempP2 =input[i + 1].normal;
		}
		tempP3 = MediumNormal; 
		output.normal = tempOrigin + tempP2 + tempP3;
		

		//float3 AdjVector1= tempP2.xyz - tempOrigin.xyz;
		//float3 AdjVector2= tempP3.xyz - tempOrigin.xyz;
		//float3 CalcNormal = normalize (cross(AdjVector1, AdjVector2));
		//output.normal = CalcNormal;


		output.position = input[i].position; 
		output.position = mul(output.position,worldMatrix); 
		output.position = mul(output.position, viewMatrix);
		output.position = mul(output.position, projectionMatrix);
		output.wpos = mul(input[i].position, worldMatrix);
		//output.normal = input[i].normal;
		output.tex= input[i].tex; 


		newTriangles.Append( output ); 
			if(i==2){
			output.position = input[0].position;
			output.position = mul(output.position,worldMatrix); 
			output.position = mul(output.position, viewMatrix);
			output.position = mul(output.position, projectionMatrix);
			
			output.wpos = mul(input[0].position, worldMatrix);
			//output.normal = input[0].normal;
			output.tex= input[0].tex; 
			
			newTriangles.Append( output ); 
		}else{
			output.position = input[i+1].position;
			output.position = mul(output.position,worldMatrix); 
			output.position = mul(output.position, viewMatrix);
			output.position = mul(output.position, projectionMatrix);
			
			output.wpos = mul(input[i + 1].position, worldMatrix);
			//output.normal = input[i + 1].normal;
			output.tex= input[i + 1].tex; 
			
			newTriangles.Append( output ); 
		}

		output.position = avg;
		output.position = mul(output.position,worldMatrix); 
		output.wpos = output.position; 
		output.position = mul(output.position, viewMatrix);
		output.position = mul(output.position, projectionMatrix);
		output.tex = MediumTex;
		//output.normal = MediumNormal;


		newTriangles.Append( output ); 
		
		newTriangles.RestartStrip( ); 
	} 
}
