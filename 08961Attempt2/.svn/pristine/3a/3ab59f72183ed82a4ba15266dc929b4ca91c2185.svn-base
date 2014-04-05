#pragma once
#include "I_SceneObject.h"
#include "BasePartition.h"

class TorrusSceneObject : 
	public I_SceneObject,
	public BasePartition
{
public:
	TorrusSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd);
	~TorrusSceneObject(void);
	void renderTheScene( GraphicsContext & p_graphicsContext);

	QuadInvoker * m_quadInvoker;
	ModelLightingClass * m_model;

private:
	string m_name;
	struct TorusBuffer
	{
		float innerRadius;
		float outerRadius;
		XMFLOAT2 padding;
	};
	struct LightPosBuffer
	{
		XMFLOAT3 LightPos;
		float rangeMod;
	};



	float updateIncrement;
	clientDataStruct* m_dataStruct;
};

