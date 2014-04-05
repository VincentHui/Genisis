#pragma once
#include "i_sceneobject.h"
#include "BasePartition.h"
class EllipsoidSceneObject :
	public I_SceneObject,
	public BasePartition
{
public:
	EllipsoidSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd);
	~EllipsoidSceneObject(void);
	void renderTheScene( GraphicsContext & p_graphicsContext);
	QuadInvoker * m_quadInvoker;
	ModelLightingClass * m_model;
	clientDataStruct * m_dataStruct;



private:
	string m_name;
	struct EllispoidBuffer
	{
		float XLen;
		float YLen;
		float ZLen;
		float padding;
	};
	float updateIncrement;

};

