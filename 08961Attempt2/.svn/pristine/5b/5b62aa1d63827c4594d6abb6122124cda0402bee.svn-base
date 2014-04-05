#pragma once
#include "i_sceneobject.h"
class AlienSceneObject :
	public I_SceneObject
{
	const static UINT swarmSize = 50;
public:
	AlienSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd);
	~AlienSceneObject(void);
	void renderTheScene( GraphicsContext & p_graphicsContext);
private:
	QuadInvoker * m_quadInvoker;
	ModelLightingClass * m_model;

	vector<int> randNums;

	


	float updateIncrement;

	struct animationBuffer
	{
		float time;
		XMFLOAT3 padding;
	};

	struct LightPosBuffer
	{
		XMFLOAT3 LightPos;
		float rangeMod;
	};
};

