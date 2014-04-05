#pragma once
#include "i_sceneobject.h"
#include "BasePartition.h"
class BezierSceneObject :
	public I_SceneObject
{
public:
	BezierSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd);
	~BezierSceneObject(void);
	void renderTheScene( GraphicsContext & p_graphicsContext);
private:
	struct TorusBuffer
	{
		float innerRadius;
		float outerRadius;
		XMFLOAT2 padding;
	};
	QuadInvoker * m_quadInvoker;
	ModelLightingClass * m_model;
	ModelLightingClass::LightingVertexType* vertices;
	float updateIncrement;

	BasePartition::clientDataStruct * m_dataStruct;

	ModelLightingClass::LightingVertexType* updateBezierPoints(float increment, float ampModifier)
	{

		vertices = new ModelLightingClass::LightingVertexType[16];
		// Row 0
		vertices[0].position = XMFLOAT3(-15.0f, sin(increment) * ampModifier, 15.0f);  //
		vertices[0].texture = XMFLOAT2(0.0f, 0.0f);//
		vertices[0].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		vertices[1].position = XMFLOAT3(-5.0f,  sin(increment) * ampModifier, +15.0f);  //
		vertices[1].texture = XMFLOAT2(1.0f, 1.0f);//
		vertices[1].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[2].position =XMFLOAT3(+5.0f,  sin(increment) * ampModifier, +15.0f); //
		vertices[2].texture = XMFLOAT2(0.0f, 1.0f);//
		vertices[2].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[3].position =XMFLOAT3(+15.0f, sin(increment) * ampModifier, +15.0f);  //
		vertices[3].texture = XMFLOAT2(0.0f, 0.0f);//
		vertices[3].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		// Row 1
		vertices[4].position = XMFLOAT3(-15.0f, sin(increment + XM_PI/2) * ampModifier, +5.0f);  //
		vertices[4].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[4].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[5].position = XMFLOAT3(-5.0f,  sin(increment + XM_PI/2) * ampModifier, +5.0f);  //
		vertices[5].texture = XMFLOAT2(1.0f, 1.0f);
		vertices[5].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[6].position =XMFLOAT3(+5.0f,  sin(increment + XM_PI/2) * ampModifier, +5.0f); //
		vertices[6].texture = XMFLOAT2(0.0f, 1.0f);
		vertices[6].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[7].position =XMFLOAT3(+15.0f, sin(increment + XM_PI/2) * ampModifier, +5.0f);  //
		vertices[7].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[7].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		// Row 2
		vertices[8].position = XMFLOAT3(-15.0f, sin(increment + XM_PI) * ampModifier, -5.0f);  //
		vertices[8].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[9].position = XMFLOAT3(-5.0f,  sin(increment + XM_PI) * ampModifier, -5.0f);  //
		vertices[9].texture = XMFLOAT2(1.0f, 1.0f);
		vertices[9].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[10].position =XMFLOAT3(+5.0f,  sin(increment + XM_PI) * ampModifier, -5.0f); //
		vertices[10].texture = XMFLOAT2(0.0f, 1.0f);
		vertices[10].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[11].position = XMFLOAT3(+15.0f, sin(increment + XM_PI) * ampModifier, -5.0f);  //
		vertices[11].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[11].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		// Row 3
		vertices[12].position = XMFLOAT3(-15.0f,  sin(increment + (3 * XM_PI)/2) * ampModifier, -15.0f);  //
		vertices[12].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[12].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[13].position = XMFLOAT3(-5.0f,  sin(increment + (3 * XM_PI)/2) * ampModifier, -15.0f);  //
		vertices[13].texture = XMFLOAT2(1.0f, 1.0f);
		vertices[13].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[14].position =XMFLOAT3(+5.0f,  sin(increment + (3 * XM_PI)/2) * ampModifier, -15.0f); //
		vertices[14].texture = XMFLOAT2(0.0f, 1.0f);
		vertices[14].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

		vertices[15].position = XMFLOAT3(+15.0f, sin(increment + (3 * XM_PI)/2) * ampModifier, -15.0f);  //
		vertices[15].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[15].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		return vertices;
	}

	static void TW_CALL FractionalEven(void *clientData)
	{
		auto clientDataPtr = static_cast< BasePartition::clientDataStruct *>(clientData);
		auto invoker = clientDataPtr->m_q;
		auto handle = clientDataPtr->m_h;
		auto device = clientDataPtr->m_dev;
		invoker->InitializeShaderProgram(device, handle, L"../Engine/BézierHS.hlsl", "FractionalEven","hs_5_0");
	}
	static void TW_CALL FractionalOdd(void *clientData)
	{
		auto clientDataPtr = static_cast< BasePartition::clientDataStruct *>(clientData);
		auto invoker = clientDataPtr->m_q;
		auto handle = clientDataPtr->m_h;
		auto device = clientDataPtr->m_dev;
		invoker->InitializeShaderProgram(device, handle, L"../Engine/BézierHS.hlsl", "FractionalOdd","hs_5_0");
	}
	static void TW_CALL Integer(void *clientData)
	{
		auto clientDataPtr = static_cast< BasePartition::clientDataStruct *>(clientData);
		auto invoker = clientDataPtr->m_q;
		auto handle = clientDataPtr->m_h;
		auto device = clientDataPtr->m_dev;
		invoker->InitializeShaderProgram(device, handle, L"../Engine/BézierHS.hlsl", "BezierHullShader","hs_5_0");
	}
};

