Texture2D shaderTexture;
SamplerState SampleType;

//type definition
struct GSOutput
{
	float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	uint VertId : SVSCALE;
};


float4 GeoExplosionPixelShader(GSOutput input) : SV_TARGET
{
	return float4(0.2f, 0.1f, 0.1f, 1.0f) * (input.VertId * 0.001);
}