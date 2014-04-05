#pragma once
#include "i_sceneobject.h"
class GeoExplosionSceneObject :
	public I_SceneObject
{
public:
	GeoExplosionSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd);
	~GeoExplosionSceneObject(void);
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
		float incrementSizeMod;;
	};
};

