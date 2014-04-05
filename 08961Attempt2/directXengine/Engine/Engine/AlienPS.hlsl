Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
	float4 specularColor;
};

cbuffer LightPosBuffer
{
	float3 LightPos;
	float rangeMod;
}

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 Wpos :SVWPOS;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

float4 AlienPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
	textureColor = shaderTexture.Sample(SampleType, input.tex); //grass


	//find attenuation
	float3 Templightpos = LightPos;
	Templightpos = Templightpos - input.Wpos.xyz;
	float d = length(Templightpos);
	Templightpos = normalize(Templightpos);
	float attenuation = 1/(d + (d <= 0 )) * rangeMod ;
	saturate(attenuation);

	color = float4(0.2, 0.2, 0.2, 1.0);
	float4 PosCol = float4(0.0, 0.0, 0.0, 1.0);
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
	
	color *= textureColor;
	PosCol *= textureColor;

	return lerp(color, PosCol, specularPower);
	return color;
}