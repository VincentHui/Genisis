
Texture2D shaderTexture;
SamplerState SampleType;

//1
cbuffer RayLightBufferType
{
	float padding;
	float3 lightPosition;
	float4 lightColour;
	float4 lightIntensity;
	float4 ambientColour;
	float4 backgroundColour;
};

//2
cbuffer ViewPortInfo
{
	float4 eyePos; //eye position
	float nearPlane; //distance from eye to image plane
	float farPlane; //distance from eye to the far plane
	float viewportW;
	float viewportH;
};

//3
cbuffer ViewInverseStruct
{
	float4x4 ViewInverse;
};

//4
cbuffer modifiers
{
	float levelVal;
	float shininess;
	float2 padding2;
};

//5
cbuffer SphereColors
{
	float4 sphereColour;
	float4 sphereColour2;
	float4 sphereColour3;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

struct Ray 
{
	float3 o; // origin
	float3 d; // direction
};

struct Sphere 
{
	float3 centre;
	float rad2; // radius^2
	float4 color;
	float Kd, Ks, Kr, shininess;
};

#define MIN_XYZ -5.0
#define MAX_XYZ 5.0
static const float3 BoxMinimum =(float3)MIN_XYZ;
static const float3 BoxMaximum =(float3)MAX_XYZ;

#define INTERVALS 100
#define PI 3.14159
#define SQR(N) (N*N)
#define NOBJECTS 10
static Sphere object[NOBJECTS] =
{
	//sphere 1
	{0.0, 0.0, 0.0, SQR(1.0), sphereColour, 1.0, 1.0, 1.0, shininess},
	//sphere 2
	{2.0, -1.0, 0.0, SQR(0.5), sphereColour2, 1.0, 1.0, 1.0,shininess},
	//sphere 4
	{-1.0, -5.0, 0.0, SQR(0.2), sphereColour2, 1.0, 1.0, 1.0,shininess},
	//sphere 5
	{5.0, -9.0, 0.0, SQR(0.1), sphereColour2, 1.0, 1.0, 1.0,shininess},
	//sphere 6
	{3.0, -3.0, 0.0, SQR(0.6), sphereColour2, 1.0, 1.0, 1.0,shininess},
	//sphere 7
	{-5.0, -1.0, 0.0, SQR(0.4), sphereColour2, 1.0, 1.0, 1.0,shininess},
	//sphere 8
	{6.0, -1.0, 0.0, SQR(0.3), sphereColour2, 1.0, 1.0, 1.0,shininess},
	//sphere 9
	{1.0, -1.0, 0.0, SQR(0.15), sphereColour2, 1.0, 1.0, 1.0,shininess},
	//sphere 10
	{23.0, -1.0, 0.0, SQR(0.01), sphereColour2, 1.0, 1.0, 1.0,shininess},
	//sphere 3
	{0.0, -201.5, 0.0, SQR(200.0), sphereColour3, 0.8, 0.3, 0.3,shininess}
};

bool IntersectBox(in Ray ray, in float3 minimum, in float3 maximum, out float timeIn, out float timeOut)
{
	float3 OMIN = ( minimum - ray.o ) / ray.d;
		float3 OMAX = ( maximum - ray.o ) / ray.d;
		float3 MAX = max ( OMAX, OMIN );
		float3 MIN = min ( OMAX, OMIN );
		timeOut = min ( MAX.x, min ( MAX.y, MAX.z ));
	timeIn = max ( max ( MIN.x, 0.0 ), max ( MIN.y, MIN.z ) );
	return timeOut > timeIn ;
}


float Function ( float3 Position )
{
	float X=Position.x;
	float Y=Position.y;
	float Z=Position.z;
	float T=PI/2.0;
	float Fun = 4.0  - cos ( Y + T * Z ) - cos ( Y - T * Z ) - cos ( Z - T * X ) - cos ( Z + T * X );
	//float Fun = 2.0 + sin(T * Y + X) - cos (X * T - Y) - sin (Z * T);
	//float Fun = 1;
	return Fun-levelVal;
}



bool IntersectSurface ( in Ray ray, in float start, in float final, out float val )
{
	float step = ( final - start ) / float ( INTERVALS );
	float time = start;
	float3 Position = ray.o + time * ray.d;
		float right, left = Function ( Position );

	for ( int i = 0; i < INTERVALS; ++i )
	{
		time += step;
		Position += step * ray.d;
		right = Function ( Position );

		if ( left * right < 0.0 )
		{
			val = time + right * step / ( left - right );
			return true;
		}

		left = right;
	}

	return false;
}

static const float3 Zero = float3 ( 0.0, 0.0, 0.0 );
static const float3 Unit = float3 ( 1.0, 1.0, 1.0 );
static const float3 AxisX = float3 ( 1.0, 0.0, 0.0 );
static const float3 AxisY = float3 ( 0.0, 1.0, 0.0 );
static const float3 AxisZ = float3 ( 0.0, 0.0, 1.0 );
#define STEP 0.01
float3 CalcNormal ( float3 Position ) 
{
	float A = Function ( Position + AxisX * STEP ) - Function ( Position - AxisX * STEP ); 
	float B = Function ( Position + AxisY * STEP ) - Function ( Position - AxisY * STEP ); 
	float C = Function ( Position + AxisZ * STEP ) - Function ( Position - AxisZ * STEP );
	return normalize ( float3 ( A, B, C ) ); 
}

float SphereIntersect(Sphere s, Ray ray, out bool hit)
{
	float t;
	float3 v = s.centre - ray.o;
		float A = dot(v, ray.d);
	float B = dot(v, v) - A*A;
	float R=sqrt(s.rad2);

	if (B>R*R)
	{
		hit = false;
		t=farPlane;
	}
	else
	{
		float disc = sqrt(R*R-B);
		t = A - disc;

		if (t<0.0)
		{
			hit = false;
		}

		else
			hit = true;
	}

	return t;
}

float3 SphereNormal(Sphere s, float3 pos)
{
	return normalize(pos - s.centre);
}

float3 NearestHit(Ray ray, out int hitobj, out bool anyhit)
{
	float mint = farPlane;
	hitobj = -1;
	anyhit = false;

	for(int i=0; i<NOBJECTS; i++) 
	{
		bool hit;
		float t = SphereIntersect(object[i], ray, hit);

		if (hit) 
		{
			if (t < mint)
			{
				hitobj = i;
				mint = t;
				anyhit = true;
			}
		}
	}

	return ray.o + ray.d*mint;
}

float4 Phong(float3 norm, float3 Ldir, float3 Vdir, float shininess, float4 diffuseColor, float4 specularColor)
{
	float NdotL = dot(norm, Ldir);
	float diff = saturate(NdotL);
	float3 rDir = reflect(Ldir, norm);
		float spec = pow(saturate(dot(Vdir, rDir)), shininess);
	return diff*diffuseColor + spec*specularColor;
}

float4 Shade( float3 hitPos, float3 normal, float3 viewDir, float3 color )
{
	float3 lightDir = normalize(lightPosition - hitPos);
		float4 diff = float4(color * 1, 0);
		float4 spec = float4(color * 1, 0);
		return lightColour*lightIntensity*Phong(normal, lightDir, viewDir, shininess, diff, spec) ;
}


float4 Raytrace ( Ray ray )
{
	Ray currRay = ray;

	int depth = 2;

	float4 result = (float4)0;
		float start, final;
	float t;

	if ( IntersectBox ( currRay, BoxMinimum, BoxMaximum, start, final ) )
	{
		for (int x = 0; x <= depth; x++)
		{
			if ( IntersectSurface ( currRay, start, final, t ) )
			{
				float3 Position = currRay.o + currRay.d * t;
					float3 normal = CalcNormal ( Position );
					float3 color = backgroundColour.xyz * ( Position - BoxMinimum ) / ( BoxMaximum - BoxMinimum );
					result += Shade(Position, normal, currRay.d, color);

				currRay.d = reflect(currRay.d, normal);
				currRay.o = Position;
				start = 0.00001;
				t = 0.0;
			}
			else
			{
				result = backgroundColour;
			}
		}
	}
	else
	{
		result = backgroundColour;
	}

	return result;
}

float4 SimplePixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	Ray eyeray;
	eyeray.o = eyePos.xyz;
	float3 dir;

	//scale the image shape such that it has the same aspect ratio as the viewport
	dir.xy = input.tex*float2(1.0, viewportH/viewportW);
	dir.z = nearPlane;

	//Eye ray direction d is specified in view space. It must be transformed back into
	//the world space:
	eyeray.d = mul(float4(dir, 0.0), ViewInverse).xyz;
	eyeray.d = normalize(eyeray.d);

	// find nearest hit
	int hitobj;
	bool hit;
	float3 norm;
	float4 RTColor = 0;
	float lightInensity = 1.0;

	RTColor = Raytrace(eyeray);

	return RTColor;
	//return float4(1.0f, 0.0f, 0.0f, 0.0f);
	return textureColor;

}