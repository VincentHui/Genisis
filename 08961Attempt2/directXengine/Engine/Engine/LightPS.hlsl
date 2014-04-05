Texture2D shaderTexture[5];
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

cbuffer texelOffset
{
	float texoffset;
	float3 padding;
};
//type definition
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 wpos : SVWPOS;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
	float scale : SVSCALE;
};

static const float PI = 3.14159265f;


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader // 
////////////////////////////////////////////////////////////////////////////////
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
	float3 reflection;
	float4 specular;

	//find attenuation
	float3 Templightpos = LightPos;
	Templightpos = Templightpos - input.wpos.xyz;

	float d = length(Templightpos);

	Templightpos = normalize(Templightpos);

	float attenuation = 1/(d + (d <= 0 )) * rangeMod ;
	saturate(attenuation);

	float offset = texoffset;
	float4 h;
	h[0] = shaderTexture[0].SampleLevel(SampleType, input.tex + float2( offset, 0), 0).r * input.scale;
	h[1] = shaderTexture[0].SampleLevel(SampleType, input.tex + float2(0, offset), 0).r * input.scale;

	float3 AdjVector1 = float3( ((input.wpos.x + offset))  , h[0], (input.wpos.z) ) -input.wpos.xyz;
	float3 AdjVector2 = float3((input.wpos.x ) , h[1], ((input.wpos.z + offset) ) ) -input.wpos.xyz;

	float3 CalcNormal = normalize( cross(AdjVector2, AdjVector1));
	input.normal = CalcNormal;



	float4 ambient = ambientColor;
	float4 grassTex = shaderTexture[1].Sample(SampleType, input.tex * float2(100.0f, 100.0f)) * ambient * float4(0.7f, 0.7f, 0.7f, 1.0f); //grass
	float4 slopeTex = shaderTexture[2].Sample(SampleType, input.tex * float2(10.0f, 10.0f)) * ambient; //slope
	float4 mountainTopTex = shaderTexture[3].Sample(SampleType, input.tex * float2(10.0f, 10.0f)) * ambient; // mountain top
	float4 variationTex = shaderTexture[4].Sample(SampleType, input.tex) * ambient;

	float slope = 1.0f - input.normal.y;
	
	////slope
	color = lerp( grassTex, variationTex, 0.2);
	color = lerp( color, slopeTex , slope);

	//mountain top
	float whiteness = smoothstep(0.5, 1.5, input.wpos.y);
	color = lerp( color, mountainTopTex , whiteness);

	float4 texcolor = color;
	float4 dirColor = color ;
	lightDir = -lightDirection;

	lightIntensity = saturate(dot(input.normal, lightDir));
	if(lightIntensity > 0.0f)
	{
		dirColor += (diffuseColor * lightIntensity);
	}
	dirColor = saturate(dirColor);
	dirColor *= texcolor;

	lightIntensity = saturate(dot(input.normal.xyz, Templightpos));
	color = float4(0,0, 0, 1.0);
	if(lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity) * attenuation;
	}

	color *= texcolor;

	return lerp(dirColor, color, specularPower);
}
