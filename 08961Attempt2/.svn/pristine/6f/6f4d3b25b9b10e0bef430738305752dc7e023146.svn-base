Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
};

cbuffer LightPosBuffer
{
	float3 LightPos;
	float rangeMod;
}

//type definition
struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float4 wpos : SVPOS;
};



////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 TorusPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
	float3 reflection;
	float4 specular;

	textureColor = shaderTexture.Sample(SampleType, input.tex); //grass

	//find attenuation
	float3 Templightpos = LightPos;
	Templightpos = Templightpos - input.wpos.xyz;
	float d = length(Templightpos);
	Templightpos = normalize(Templightpos);
	float attenuation = 1/((d * 2.0f) + (d <= 0 )) * rangeMod;
	saturate(attenuation);

	color = float4(0.1, 0.1, 0.1, 1.0);
	float4 PosCol = float4(0.005, 0.005, 0.005, 1.0);
	//specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    lightDir = -lightDirection;
    lightIntensity = saturate(dot(input.normal, lightDir));
	 if(lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity) * 0.8f;

    }

	lightIntensity = saturate(dot(input.normal.xyz, Templightpos));
	if(lightIntensity > 0.0f)
	{
		PosCol += (diffuseColor * lightIntensity) * attenuation;
	}

    //color = saturate(color + specular);
	color *= textureColor;
	PosCol *= textureColor;

	//return textureColor;
	return lerp(color, PosCol, specularPower);
	return color;
}
