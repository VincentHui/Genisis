//globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct HullOutputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};



float3 Bilerp(float3 v0, float3 v1, float3 v2, float3 v3, float2 i)
{
    float3 bottom = lerp(v0, v3, i.x);
    float3 top    = lerp(v1, v2, i.x);
    float3 result = lerp(bottom, top, i.y);
	return result;
}

float3 TessellatedWorldPos(ConstantOutputType input, 
                    float2 UV : SV_DomainLocation,
                    const OutputPatch<HullOutputType, 4> terrainQuad)
{
    // bilerp the position
    float3 worldPos = Bilerp(terrainQuad[0].position, terrainQuad[1].position, terrainQuad[2].position, terrainQuad[3].position, UV);
    
	return worldPos;
}

float4 BernsteinBasis(float t)
{
    float invT = 1.0f - t;

    return float4( invT * invT * invT,
                   3.0f * t * invT * invT,
                   3.0f * t * t * invT,
                   t * t * t );
}

//--------------------------------------------------------------------------------------
float4 dBernsteinBasis(float t)
{
    float invT = 1.0f - t;

    return float4( -3 * invT * invT,
                   3 * invT * invT - 6 * t * invT,
                   6 * t * invT - 3 * t * t,
                   3 * t * t );
}

float3 EvaluateBezier( const OutputPatch<HullOutputType, 16> bezpatch,
                       float4 BasisU,
                       float4 BasisV )
{
    float3 Value = float3(0,0,0);
    Value  = BasisV.x * ( bezpatch[0].position * BasisU.x + bezpatch[1].position * BasisU.y + bezpatch[2].position * BasisU.z + bezpatch[3].position * BasisU.w );
    Value += BasisV.y * ( bezpatch[4].position * BasisU.x + bezpatch[5].position * BasisU.y + bezpatch[6].position * BasisU.z + bezpatch[7].position * BasisU.w );
    Value += BasisV.z * ( bezpatch[8].position * BasisU.x + bezpatch[9].position * BasisU.y + bezpatch[10].position * BasisU.z + bezpatch[11].position * BasisU.w );
    Value += BasisV.w * ( bezpatch[12].position * BasisU.x + bezpatch[13].position * BasisU.y + bezpatch[14].position * BasisU.z + bezpatch[15].position * BasisU.w );

    return Value;
}


float3 filterNormal(float2 uv, float texelSize, float texelAspect)
{
	float4 h;
	h[0] = shaderTexture.SampleLevel(SampleType, uv + texelSize*float2( 0,-1), 0).r * texelAspect;
	h[1] = shaderTexture.SampleLevel(SampleType, uv + texelSize*float2(-1, 0), 0).r * texelAspect;
	h[2] = shaderTexture.SampleLevel(SampleType, uv + texelSize*float2( 1, 0), 0).r * texelAspect;
	h[3] = shaderTexture.SampleLevel(SampleType, uv + texelSize*float2( 0, 1), 0).r * texelAspect;
	float3 n;
	n.z = h[0] - h[3];
	n.x = h[1] - h[2];
	n.y = 1;
	return normalize(n);
}


[domain("quad")]
PixelInputType BezierDomainShader(ConstantOutputType input, float2 uv : SV_DomainLocation, const OutputPatch<HullOutputType, 16> bezpatch)
{
    PixelInputType output;
 
	float4 BasisU = BernsteinBasis( uv.x );
    float4 BasisV = BernsteinBasis( uv.y );
	float4 dBasisU = dBernsteinBasis( uv.x );
    float4 dBasisV = dBernsteinBasis( uv.y );

	float3 WorldPos = EvaluateBezier( bezpatch, BasisU, BasisV );
	float3 Tangent = EvaluateBezier( bezpatch, dBasisU, BasisV );
    float3 BiTangent = EvaluateBezier( bezpatch, BasisU, dBasisV );
    float3 Norm = normalize( cross( Tangent, BiTangent ) );

	output.normal = Norm;
	output.tex = uv;

	output.position = mul(float4( WorldPos.xyz, 1), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    return output;
}