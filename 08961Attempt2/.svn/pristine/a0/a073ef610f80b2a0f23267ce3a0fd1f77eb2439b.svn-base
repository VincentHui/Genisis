#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureHandler.h"
#include <fstream>
#include <AntTweakBar.h>
#include "LightInfo.h"
#include "Camera.h"
#include "Entity.h"
#include <unordered_map>
#include <sstream>

using namespace std;
using namespace DirectX;
class ModelLightingClass : Entity
{
private:
	//struct for buffer

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	//methods
	bool InitializeBuffers(ID3D11Device* device, bool DrawInverted);
	void ShutdownBuffers();

	// Release the model data.
	void ReleaseModel();
	bool LoadModel(char* modelName);
	bool LoadBlenderOBJ(char* modelName);

	//member variables
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	//array of ModelType structs
	ModelType* m_model;
	bool invert;

	XMVECTOR Quaternion;

	//transform variables
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	XMMATRIX model_ModelWorld;
	XMMATRIX model_Rotation;
	XMMATRIX model_Scale;
	XMMATRIX model_Translation;


public:
	ID3D11Buffer* GetVertexBuffer()
	{
		return m_vertexBuffer;
	}
	ID3D11Buffer* GetindexBuffer()
	{
		return m_indexBuffer;
	}
	TwBar * m_bar;
	TwBar * getBar()
	{
		return m_bar;
	}

	void SetPos(const XMFLOAT3 & p_in)
	{
		Position = p_in;
	};
	const XMFLOAT3 & GetPos()
	{
		return Position;
	}

	void SetRot(const XMFLOAT3 & p_in)
	{
		Rotation = p_in;
	};
	void SetScale(const XMFLOAT3 & p_in)
	{
		Scale = p_in;
	};

	struct LightingVertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	ModelLightingClass(string barName, bool p_quad, const XMFLOAT3 & postion);
	ModelLightingClass(const ModelLightingClass& other);
	~ModelLightingClass(void);

	template<class objectTemplate>
	void RenderAllBuffers(ID3D11DeviceContext* deviceContext, enum D3D_PRIMITIVE_TOPOLOGY p_enum)
	{
		unsigned int stride;
		unsigned int offset;

		// Set vertex buffer stride and offset.
		stride = sizeof(objectTemplate); 
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(p_enum);
	}
	template<class VerticesTemplate>
	bool InitializeAllBuffers(ID3D11Device* device, VerticesTemplate p_vertices[], unsigned long p_indices[])
	{
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VerticesTemplate) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;
		vertexData.pSysMem = p_vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = p_indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if(FAILED(result))
		{
			return false;
		}
		return true;
	}

	bool Initialize(ID3D11Device* device, char* modelName, WCHAR** textureName, HWND hwnd, bool invert);
	void Shutdown();
	void update();
	void ModelMatrix(XMMATRIX & p_out);
	const XMMATRIX & ModelMatrix();

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetBaseTexture();

	unordered_map<string, float> TweakVariables;
	unordered_map<string, XMFLOAT3> ThreeFloatTweakVariables;
	unordered_map<string, TextureHandler*> m_textures;

	void BuildQuadPatch(ID3D11Device* md3dDevice)
	{

	LightingVertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new LightingVertexType[16];
	// Create the index array.
	indices = new unsigned long[16];
	m_indexCount = 16;
	m_vertexCount = 16;

	// Row 0
	vertices[0].position = XMFLOAT3(-10.0f, -10.0f, +15.0f);  //
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);//
	vertices[0].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	vertices[1].position = XMFLOAT3(-5.0f,  0.0f, +15.0f);  //
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);//
	vertices[1].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[2].position =XMFLOAT3(+5.0f,  0.0f, +15.0f); //
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);//
	vertices[2].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[3].position =XMFLOAT3(+10.0f, 0.0f, +15.0f);  //
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);//
	vertices[3].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	// Row 1
	vertices[4].position = XMFLOAT3(-15.0f, 0.0f, +5.0f);  //
	vertices[4].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[4].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[5].position = XMFLOAT3(-5.0f,  0.0f, -5.0f);  //
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[5].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[6].position =XMFLOAT3(+5.0f,  20.0f, +5.0f); //
	vertices[6].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[6].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[7].position =XMFLOAT3(+15.0f, 0.0f, +5.0f);  //
	vertices[7].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[7].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// Row 2
	vertices[8].position = XMFLOAT3(-15.0f, 0.0f, -5.0f);  //
	vertices[8].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[9].position = XMFLOAT3(-5.0f,  0.0f, -5.0f);  //
	vertices[9].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[9].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[10].position =XMFLOAT3(+5.0f,  0.0f, -5.0f); //
	vertices[10].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[10].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[11].position = XMFLOAT3(+15.0f, 0.0f, -5.0f);  //
	vertices[11].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[11].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	// Row 3
	vertices[12].position = XMFLOAT3(-10.0f, 10.0f, -15.0f);  //
	vertices[12].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[12].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[13].position = XMFLOAT3(-5.0f,  0.0f, -15.0f);  //
	vertices[13].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[13].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[14].position =XMFLOAT3(+5.0f,  0.0f, -15.0f); //
	vertices[14].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[14].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;

	vertices[15].position = XMFLOAT3(+25.0f, 10.0f, -15.0f);  //
	vertices[15].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[15].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);;


	// Load the index array with data.
	for(i=0; i<m_indexCount; i++)
	{
		indices[i] = i;
	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(LightingVertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = md3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = md3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;


	}

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

	void AddModel3FloatTweakVar(const XMFLOAT3 & p_in, string name, string def)
	{
		std::unordered_map<std::string, float>::const_iterator got = TweakVariables.find (name);
		if(got != TweakVariables.end())
		{
			return;
		}
		ThreeFloatTweakVariables[name] = p_in;
		string tempName = name + "x";
		TwAddVarRW(m_bar, tempName.c_str(), TW_TYPE_FLOAT, &ThreeFloatTweakVariables[name].x, def.c_str());
		tempName = name + "y";
		TwAddVarRW(m_bar, tempName.c_str(), TW_TYPE_FLOAT, &ThreeFloatTweakVariables[name].y, def.c_str());
		tempName = name + "z";
		TwAddVarRW(m_bar, tempName.c_str(), TW_TYPE_FLOAT, &ThreeFloatTweakVariables[name].z, def.c_str());
	}

	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, LightingVertexType * lightIn)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightingVertexType* verticesPtr;
	HRESULT result;

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (LightingVertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)lightIn, (sizeof(LightingVertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	return true;
}
};

