Texture2D shaderTexture;
SamplerState SampleType;

//type definition
struct GSOutput
{
	float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	uint VertId : SVSCALE;
};


float4 GeoSmokePixelShader(GSOutput input) : SV_TARGET
{
	 float4 textureColor;
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	textureColor.w = 1.0;


    //return textureColor;

	return float4(0.1f, 0.1f, 0.1f, 1.0f) * (input.VertId * 0.001) * textureColor;
}