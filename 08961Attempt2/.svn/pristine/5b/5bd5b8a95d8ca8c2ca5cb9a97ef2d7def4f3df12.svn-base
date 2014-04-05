#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <AntTweakBar.h>
#include <sstream>
#include "LightInfo.h"
#include "d3dclass.h"
#include "Camera.h"

using namespace std;
using namespace DirectX;

class GraphicsContext
{
public:
	GraphicsContext(void);
	~GraphicsContext(void);
	void SetViewContext( XMMATRIX * p_in)
	{
		c_view = p_in;
	}

	XMMATRIX * GetViewContext()
	{
		return c_view;
	}

	void SetWorldContext( XMMATRIX * p_in)
	{
		c_world = p_in;
	}

	XMMATRIX * GetWorldContext()
	{
		return c_world;
	}
	void SetProjectionContext( XMMATRIX * p_in)
	{
		c_projection = p_in;
	}

	XMMATRIX * GetProjectionContext()
	{
		return c_projection;
	}
	void SetD3DContext( D3DClass * p_in)
	{
		c_d3d = p_in;
	}
	D3DClass* GetD3DContext()
	{
		return c_d3d;
	}

	//keeps these scoped objects avaliable
	//these values are both singular and global
	XMMATRIX* c_view;
	XMMATRIX* c_world;
	XMMATRIX* c_projection;
	D3DClass* c_d3d;
	LightInfo* c_lightInfo;
	Camera * c_cam;
	float windowWidth;
	float windowHeight;
	float nearPlane;
	float farPlane;
	float deltaTime;
private:

};

