Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
	float3 LightPos;
	float padding;
};

//type definition
struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 EllipsoidPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
	float3 reflection;
	float4 specular;

	color = float4(0.3, 0.3, 0.3, 1.0);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    lightDir = -lightDirection;
    lightIntensity = saturate(dot(input.normal, lightDir));
	 if(lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity) * 0.8f;
    }

    color = saturate(color + specular);
	return color;
}
