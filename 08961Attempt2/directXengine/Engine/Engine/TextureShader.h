////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
//#include <d3dx11async.h>
#include <d3dcompiler.h>
#include "Logger.h"
#include <fstream>
using namespace std;
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShader
{
private:
	struct MatrixBufferType
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 projectionMatrix;
	};

public:
	TextureShader();
	TextureShader(const TextureShader&);
	~TextureShader();

	bool Initialize(ID3D11Device* _device, HWND _handle);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection, ID3D11ShaderResourceView* _texture);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR* vertexShader, WCHAR* pixelShader);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection, ID3D11ShaderResourceView* _texture);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

    //There is a new private variable for the sampler state pointer. This pointer will be used to interface with the texture shader. 

	ID3D11SamplerState* m_sampleState;
};

#endif

