cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer animationBuffer
{
	float time;
	float3 padding;
};


struct VertexInputType
{
	float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 Wpos :SVWPOS;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

PixelInputType AlienVertexShader( VertexInputType input)
{
    PixelInputType output;
    
    input.position.w = 1.0f;


	if (abs( input.position.x) > 10)
	{
		input.position.x += input.position.x;
	}

	if (input.position.y > 2 && input.position.y < 3 )
	{if(input.position.z>1.5){
		input.position.z += input.position.z* 2;
		input.position.y += sin(time * 100)*1000*smoothstep(20,100, input.position.z);
	}

	if(input.position.z<-1.5){
		input.position.z += input.position.z* 2;
		input.position.y += sin(time * 100)*1000*smoothstep(20,100, -input.position.z);
	}

	}

	if(abs( input.position.z) > 2 && abs( input.position.z) < 3)
	{
		input.position.z += input.position.z;
	}

		//input.position.y += sin(time );
	//if (abs( input.position.z) > 5 && abs(input.position.y) > 3)
	//{
	//	input.position.z += input.position.z;
	//}

    output.position = mul(input.position, worldMatrix);
	output.Wpos = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = input.tex;

    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);


    return output;
}