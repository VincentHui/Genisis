#pragma once
#include "I_SceneObject.h"
class SmokeSceneObject :
	public I_SceneObject
{
public:
	SmokeSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd);
	~SmokeSceneObject(void);
	void renderTheScene( GraphicsContext & p_graphicsContext);
private:
	QuadInvoker * m_quadInvoker;
	ModelLightingClass * m_model;
	float RadIncrement;
	struct particleStruct
	{
		float size;
		float time;
		float incrementMod;
		float heightToKill;
	};
};

