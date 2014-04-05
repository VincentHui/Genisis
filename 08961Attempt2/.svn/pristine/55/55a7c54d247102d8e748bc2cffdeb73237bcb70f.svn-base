
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Logger.h"
#include <AntTweakBar.h>

using namespace DirectX;

class D3DClass
{

public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	D3D11_TEXTURE2D_DESC g_DepthStencilDesc;
	ID3D11DepthStencilView* m_depthStencilView;

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);
	void GetVideoCardInfo(char*, int&);
	void Resize(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetBackBufferRenderTarget();
	void ResetViewport();

	void TurnZBufferOn();
	void TurnZBufferOff();

	void EnableAlphaBlending();
	void DisableAlphaBlending();

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;

	D3D11_DEPTH_STENCIL_DESC    m_DepthStencilDesc;
	ID3D11RasterizerState* m_rasterState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	D3D11_VIEWPORT m_viewport;
	
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
	
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
};

#endif