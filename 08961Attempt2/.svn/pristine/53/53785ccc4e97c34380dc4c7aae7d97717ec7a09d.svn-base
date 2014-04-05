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
float4 BezierPixelShader(PixelInputType input) : SV_TARGET
{

    float3 lightDir;
    float lightIntensity;
    float4 color;

 //   //color = ambientColor;
	color = float4(0.3, 0.6, 0.3, 0) * float4(0.2, 0.2, 0.2, 1.0);


    lightDir = normalize(-lightDirection);


 //   // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

	 if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity) * 0.8f;

		// Saturate the ambient and diffuse color.
        color = saturate(color);

    }

	//color = shaderTexture.SampleLevel(SampleType, input.UV, 0).r;
	//return float4( input.normal, 1.0f);
	//return float4 (1.0f, 1.0f ,0.0f, 1.0f);
	return color;
}
