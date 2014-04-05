#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h> /* remember to include the lib file! */
#include "Logger.h"
#include <fstream>

using namespace std;
using namespace DirectX;

class LightShader
{
private:
	struct MatrixBufferType
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

	//padding issue in prevoius shader?
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};

	bool InitializeShader(ID3D11Device*, HWND, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, 
	ID3D11ShaderResourceView*, XMFLOAT3 direction, XMFLOAT4 diffuse, XMFLOAT4 ambient, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower);
	void RenderShader(ID3D11DeviceContext* context, int p_indexCount);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	//buffer of lights color and direction from LightInfo object
	ID3D11Buffer* m_lightBuffer;
	//used for setting the camera position to the vertex shader
	ID3D11Buffer* m_cameraBuffer;

public:
	LightShader(void);
	LightShader(const LightShader& other);
	~LightShader(void);
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* context, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, 
		ID3D11ShaderResourceView* texture, XMFLOAT3 direction, XMFLOAT4 diffuse, XMFLOAT4 ambient, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower);
};

