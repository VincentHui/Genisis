#pragma once
#include "I_SceneObject.h"
#include <math.h>
#include "BasePartition.h"

class TerrainSceneObject : public I_SceneObject
{
public:
	TerrainSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device, HWND hwnd);
	~TerrainSceneObject(void);
	void renderTheScene( GraphicsContext & p_graphicsContext);
private:

	BasePartition::clientDataStruct * m_dataStruct;
	static void TW_CALL FractionalEven(void *clientData)
	{
		auto clientDataPtr = static_cast< BasePartition::clientDataStruct *>(clientData);
		auto invoker = clientDataPtr->m_q;
		auto handle = clientDataPtr->m_h;
		auto device = clientDataPtr->m_dev;
		invoker->InitializeShaderProgram(device, handle, L"../Engine/LightHS.hlsl", "FractionalEven","hs_5_0");
	}
	static void TW_CALL FractionalOdd(void *clientData)
	{
		auto clientDataPtr = static_cast< BasePartition::clientDataStruct *>(clientData);
		auto invoker = clientDataPtr->m_q;
		auto handle = clientDataPtr->m_h;
		auto device = clientDataPtr->m_dev;
		invoker->InitializeShaderProgram(device, handle, L"../Engine/LightHS.hlsl", "FractionalOdd","hs_5_0");
	}
	static void TW_CALL Integer(void *clientData)
	{
		auto clientDataPtr = static_cast< BasePartition::clientDataStruct *>(clientData);
		auto invoker = clientDataPtr->m_q;
		auto handle = clientDataPtr->m_h;
		auto device = clientDataPtr->m_dev;
		invoker->InitializeShaderProgram(device, handle, L"../Engine/LightHS.hlsl", "LightHullShader","hs_5_0");
	}

	struct LightPosBuffer
	{
		XMFLOAT3 LightPos;
		float rangeMod;
	};

	struct LightBufferType3
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float switcher;
	};

	struct TesDisplacementBufferType
	{
		float bias;
		float scale;
		XMFLOAT2 XY;
	};

	struct texelOffset
	{
		float texoffset;
		XMFLOAT3 padding;
	};
	string m_name;

	QuadInvoker * m_quadInvoker;
	ModelLightingClass * m_model;
};

