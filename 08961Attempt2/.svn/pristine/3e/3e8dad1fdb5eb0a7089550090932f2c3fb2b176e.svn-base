

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h> /* remember to include the lib file! */
#include <fstream>
#include "Logger.h"
#include "LightInfo.h"
#include <unordered_map>
#include "Entity.h"
#include <array>

using namespace DirectX;
using namespace std;

//const XMFLOAT4 manualTess = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

#ifndef QuadInvoker_H
#define QuadInvoker_H

class QuadInvoker : Entity
{


public:

	//D3D11_INPUT_ELEMENT_DESC posLayout()
	//{
	//	D3D11_INPUT_ELEMENT_DESC position;
	//	position.SemanticName = "POSITION";
	//	position.SemanticIndex = 0;
	//	position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//	position.InputSlot = 0;
	//	position.AlignedByteOffset = 0;
	//	position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//	position.InstanceDataStepRate = 0;
	//	return position;
	//}

	//D3D11_INPUT_ELEMENT_DESC texCoLayout()
	//{
	//	D3D11_INPUT_ELEMENT_DESC texCo;
	//	texCo.SemanticName = "TEXCOORD";
	//	texCo.SemanticIndex = 0;
	//	texCo.Format = DXGI_FORMAT_R32G32_FLOAT;
	//	texCo.InputSlot = 0;
	//	texCo.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	//	texCo.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//	texCo.InstanceDataStepRate = 0;
	//	return texCo;

	//}

	//D3D11_INPUT_ELEMENT_DESC normalLayout()
	//{
	//	D3D11_INPUT_ELEMENT_DESC normal;
	//	normal.SemanticName = "NORMAL";
	//	normal.SemanticIndex = 0;
	//	normal.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//	normal.InputSlot = 0;
	//	normal.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	//	normal.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//	normal.InstanceDataStepRate = 0;
	//	return normal;
	//}

	QuadInvoker(void);
	QuadInvoker(const QuadInvoker&);
	~QuadInvoker(void);

	struct HSL
{
	float Hue;
	float Brightness;
	float Contrast;
	float Saturation;
	float influence;
	XMFLOAT3 padding;
};

	struct MatrixBufferType
	{
		XMMATRIX  worldMatrix;
		XMMATRIX  viewMatrix;
		XMMATRIX  projectionMatrix;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct TessellationBufferType
	{
		float tessellationAmount;
		XMFLOAT3 cameraPosition;
		XMFLOAT3 misc;
		float LODModifier;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
	};

	bool InitializeVertexShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, LPCSTR vsEntry, bool simpleQuad);
	bool InitializeShaderProgram(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, LPCSTR vsEntry, LPCSTR ShaderModel);
	bool InitializeFxProgram(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, LPCSTR vsEntry, LPCSTR ShaderModel);
	bool InitializeCBuffer(ID3D11Device* device, char* bufferName, UINT sizeOfStruct);
	bool InitializeSampler(ID3D11Device* device, char* SamplerName);
	bool MapMatrixBuffer(ID3D11DeviceContext* p_DeviceContext, char* bufferName, MatrixBufferType * p_matBuffer);
	bool BufferMapFind(string p_name)
	{
		std::unordered_map<std::string, ID3D11Buffer*>::const_iterator got = bufferMap.find (p_name);
		if(got == bufferMap.end())
		{
			return false;
		}
		return true;
	}
	template<class T>
	T* MapCBuffer(ID3D11DeviceContext* p_DeviceContext, string bufferName)
	{
		// Lock the constant buffer so it can be written to.

		std::unordered_map<std::string, ID3D11Buffer*>::const_iterator got = bufferMap.find (bufferName);
		if(got == bufferMap.end())
		{
			return false;
		}

		HRESULT result = p_DeviceContext->Map(got->second, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mappedResource);
		if(FAILED(result))
		{
			Logger::AppendErrorToLog("p_DeviceContext couldn't Map, at QuadInvoker");
			return false;
		}
		//p_matBuffer = (T*)m_mappedResource.pData;

		//return true;
		return (T*)m_mappedResource.pData;
	}
	bool UnMapCBuffer(ID3D11DeviceContext* p_DeviceContext, string bufferName);
	void RenderShader(ID3D11DeviceContext*, int);
	unordered_map<string, ID3D11Buffer*> GetBufferMap();
private:

	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	//D3D11_INPUT_ELEMENT_DESC* position;
	ID3D11HullShader* m_hullShader;
	ID3D11DomainShader* m_domainShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11VertexShader* m_vertexShader;
	ID3D11SamplerState* m_sampleState;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;

	unordered_map<string, ID3D11Buffer*> bufferMap;
	unordered_map<string, ID3D11SamplerState*>samplerMap;

	D3D11_MAPPED_SUBRESOURCE m_mappedResource;
};

#endif

