#pragma once
#include "I_SceneObject.h"
#include "rendertextureclass.h"
#include "TextureShader.h"


class RayMarchSceneObject :
	public I_SceneObject
{
public:
	RayMarchSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd, int screenWidth, int screenHeight);
	~RayMarchSceneObject(void);
	void renderTheScene( GraphicsContext & p_graphicsContext);
	void deleteDerived();

private:
	void renderRayCam();

	struct orthoType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct SphereColors
	{
		XMFLOAT4 sphereColour;
	XMFLOAT4 sphereColour2;
	XMFLOAT4 sphereColour3;
	};

	struct RayLightBufferType
	{
		float padding;
		XMFLOAT3 lightPosition;
		XMFLOAT4 lightColour;
		XMFLOAT4 lightIntensity;
		XMFLOAT4 ambientColour;
		XMFLOAT4 backgroundColour;
	};

	struct ViewPortInfo
	{
		XMFLOAT4 eyePos; //eye position
		float nearPlane; //distance from eye to image plane
		float farPlane; //distance from eye to the far plane
		float viewportW;
		float viewportH;
	};

	struct ViewInverseStruct
	{
		XMMATRIX InverseView;
	};

	struct modifiers
	{
		float levelVal;
		float shininess;
		XMFLOAT2 padding;
	};

	int pastWidth, pastHeight;

	QuadInvoker * m_quadInvoker;
	ModelLightingClass * m_model;

	XMFLOAT3 RayCamPos;
	XMFLOAT3 RayCamRot;
	XMFLOAT3 RayCamLookat;
	XMMATRIX RayInverseViewMatrix;
	
	XMFLOAT4 BackColor;
	XMFLOAT4 SphereColorsToG[3];
	float updateIncrement;

	XMVECTOR q;
};

