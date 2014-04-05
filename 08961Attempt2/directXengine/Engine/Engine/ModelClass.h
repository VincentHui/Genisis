#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include "Logger.h"
#include "TextureHandler.h"


using namespace DirectX;

class ModelClass
{
public:
	ModelClass(void);
	ModelClass(const ModelClass&);
	~ModelClass(void);

	bool Initialize(ID3D11Device*, WCHAR* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	void Update(void);
	float updateValue;
	XMMATRIX model_ModelWorld;
	int GetIndexCount();
	float rot;
	//can pass its own texture resource
	ID3D11ShaderResourceView* GetTexture();

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct VertexTypeTexture
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	struct VertexTypeColor
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};


	XMMATRIX model_Rotation;
	XMMATRIX model_Scale;
	XMMATRIX model_Translation;

	//handle shutdown of buffers
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	//buffers
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	//handle texture
	bool LoadTexture(ID3D11Device* device_context, WCHAR* texture_name);
	void ReleaseTexture();

	TextureHandler* m_Texture;
	float increment;

};

#endif
