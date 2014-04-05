#pragma once
#include <AntTweakBar.h>
#include "QuadInvoker.h"

class BasePartition
{
public:
	BasePartition(void);
	virtual ~BasePartition(void);

	struct clientDataStruct
	{
		QuadInvoker * m_q;
		HWND m_h;
		ID3D11Device* m_dev;
	};

	static void TW_CALL FractionalEven(void *clientData)
	{
		auto clientDataPtr = static_cast< clientDataStruct *>(clientData);
		auto invoker = clientDataPtr->m_q;
		auto handle = clientDataPtr->m_h;
		auto device = clientDataPtr->m_dev;
		invoker->InitializeShaderProgram(device, handle, L"../Engine/TorusHS.hlsl", "TorusHullShaderFractionalEven","hs_5_0");
	}

	static void TW_CALL FractionalOdd(void *clientData)
	{
		auto clientDataPtr = static_cast< clientDataStruct *>(clientData);
		auto invoker = clientDataPtr->m_q;
		auto handle = clientDataPtr->m_h;
		auto device = clientDataPtr->m_dev;
		invoker->InitializeShaderProgram(device, handle, L"../Engine/TorusHS.hlsl", "TorusHullShaderFractionalOdd","hs_5_0");
	}

	static void TW_CALL FractionalInteger(void *clientData)
	{
		auto clientDataPtr = static_cast< clientDataStruct *>(clientData);
		auto invoker = clientDataPtr->m_q;
		auto handle = clientDataPtr->m_h;
		auto device = clientDataPtr->m_dev;
		invoker->InitializeShaderProgram(device, handle, L"../Engine/TorusHS.hlsl", "TorusHullShader","hs_5_0");
	}

};

