#ifndef HLSL_INVOKER_H
#define HLSL_INVOKER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h> /* remember to include the lib file! */
#include <fstream>
#include "Logger.h"

using namespace DirectX;
using namespace std;


class HLSL_invoker
{
public:
	HLSL_invoker(void);
	HLSL_invoker(const HLSL_invoker&);
	~HLSL_invoker(void);

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	//The render function sets the shader parameters and then draws the prepared model vertices using the shader
	bool Render(ID3D11DeviceContext*, int,  XMFLOAT4X4 ,  XMFLOAT4X4 ,  XMFLOAT4X4 );

private:
		struct MatrixBufferType
	{
		 XMFLOAT4X4  worldMatrix;
		 XMFLOAT4X4  viewMatrix;
		 XMFLOAT4X4  projectionMatrix;
	};

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*,  XMFLOAT4X4 ,  XMFLOAT4X4 ,  XMFLOAT4X4 );
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

};
#endif 


