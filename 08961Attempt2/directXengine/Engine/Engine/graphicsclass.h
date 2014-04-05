////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <AntTweakBar.h>

///////////////////////
// MY CLASS INCLUDES // 
///////////////////////
#include "d3dclass.h"
#include "Camera.h"
#include "LightInfo.h"
#include "GraphicsContext.h"
#include "SceneObjectIncludes.h"
#include "orthowindowclass.h"
#include <sstream>


using namespace std;

class GraphicsClass
{
public:
	GraphicsClass(int screenWidth, int screenHeight, HWND hwnd, D3DClass *, float nearPlane, float farPlane);
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int mouseX, int mouseY, XMFLOAT3 pos, XMFLOAT3 rot, float width, float height, float time);
	bool Render(float, float screenWidth, float screenHeight, const float & deltaTime);
	Camera* m_camera;
	ID3D11Device* GetD3DDevice();
	TwBar *m_bar;
	XMVECTOR Quaternion;
	void GetQuat(XMVECTOR *ptr);
	D3DClass* m_D3D;
	ID3D11RasterizerState* m_rasterState;
	D3D11_RASTERIZER_DESC rasterDescSolid;
	D3D11_RASTERIZER_DESC rasterDescWire;
private:

	int renderMode;

	unordered_map<string, I_SceneObject*> m_sceneObjects;

	LightInfo* m_LightInfo;

	float m_near;
	float m_far;
	RenderTextureClass * m_renderTexture;
	OrthoWindowClass * m_ortho;
	QuadInvoker * m_texShader;
	void renderTexture();
	float rotation;


	unordered_map<string, float> TweakVariables;
	void AddModelTweakVar(float p_in, string name, string def)
	{
		std::unordered_map<std::string, float>::const_iterator got = TweakVariables.find (name);
		if(got != TweakVariables.end())
		{
			return;
		}
		TweakVariables[name] = p_in;
		TwAddVarRW(m_bar, name.c_str(), TW_TYPE_FLOAT, &TweakVariables[name], def.c_str());
	}

	struct clientDataStruct
	{
		D3DClass * m_d3d;
		D3D11_RASTERIZER_DESC * m_desc;
		ID3D11RasterizerState * m_rasterizerState;
	};
	clientDataStruct * clientData;
	clientDataStruct * clientData2;

	static void TW_CALL rasterStateSolid(void *clientData)
	{
		auto data = static_cast<GraphicsClass *>(clientData);
		auto deviceContext = data->m_D3D->GetDeviceContext();
		auto device = data->m_D3D->GetDevice();
		device->CreateRasterizerState(&data->rasterDescSolid, &data->m_rasterState);
		deviceContext->RSSetState(data->m_rasterState);
	}
	
	static void TW_CALL rasterStateWire(void *clientData)
	{
		auto data = static_cast<GraphicsClass *>(clientData);
		auto deviceContext = data->m_D3D->GetDeviceContext();
		auto device = data->m_D3D->GetDevice();
		device->CreateRasterizerState(&data->rasterDescWire, &data->m_rasterState);
		deviceContext->RSSetState(data->m_rasterState);
	}
};

#endif