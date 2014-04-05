#include "ModelClass.h"


ModelClass::ModelClass(void): updateValue(0.0f), increment(0.05f), rot(0.000f), 
	model_ModelWorld(XMMatrixIdentity()), 
	model_Rotation(XMMatrixIdentity()), 
	model_Scale(XMMatrixIdentity()), 
	model_Translation(XMMatrixIdentity()),
	m_Texture(0)
{
	//set buffer pointers to zero
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass(void)
{
}

bool ModelClass::Initialize(ID3D11Device*device, WCHAR* textureFilename)
{
	bool result;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if(!result)
	{
		Logger::AppendErrorToLog("buffers won't initialize");
		return false;
	}

	//load texture for model
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		Logger::AppendErrorToLog("LoadTexture has failed at ModelClass");
		return false;
	}
	return true;
}

void ModelClass::Update(void)
{
	rot += 0.01f;
	if(rot > 6.28f)
		rot = 0.0f;

	//model_ModelWorld = XMMatrixIdentity();

	//model_Translation = XMMatrixTranslation(0.0f,0.0f, rot);
	//XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//model_Rotation = XMMatrixRotationAxis(rotaxis, rot);

	//model_ModelWorld = model_Rotation;
	//model_ModelWorld *= model_Rotation;

	//return model_ModelWorld;
	////worldMatrix *= transMatrix;
	//XMMATRIX test;
	//return test;

	updateValue += increment;
	if(updateValue < 0.0f)
	{
		increment *= -1;
	}
	if(updateValue > 1.0f)
	{	
		increment *= -1;
	}
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

//called from graphicsClass::render
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
	return;

}

void ModelClass::Shutdown()
{
	//relase texture
	ReleaseTexture();
	//release vertex and index buffers
	ShutdownBuffers();
	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

//KEEP NOTE OF THIS!!! 
//This method will read in a models vertices and then send it to the Vertex buffer
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	VertexTypeColor* verticesColor;

	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	//manually sending vertices to the vertex buffer
	// Set the number of vertices in the vertex array.
	m_vertexCount = 8;
	// Set the number of indices in the index array.
	m_indexCount = 24;

	//create vertex array
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		Logger::AppendErrorToLog("Vertex Type vertices failed to create");
		return false;
	}

	//create index array
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		Logger::AppendErrorToLog("indices array failed to create");
		return false;
	}

	//!!!! DO THIS CLOCKWISE !!!!! or lose the surface to back face culling because it is facing opposite direction
	//lol disregard that you can create the transparent effect with messed normals by 
	//using anti clockwise notation in the index array

	// Load the vertex array with data.
	//texture time!!
	//vertices[0].position = XMFLOAT3(-1.0f, -1.0f, -5.0f);  // Bottom left.
	//vertices[0].texture = XMFLOAT2(0.0f, 1.0f);


	//vertices[1].position = XMFLOAT3(0.0f, 1.0f, -5.0f);  // Top middle.
	//vertices[1].texture = XMFLOAT2(0.5f, 0.0f);


	//vertices[2].position = XMFLOAT3(1.0f, -1.0f, -5.0f);  // Bottom right.
	//vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	////triangle strip time!!
	//verticesColor = new VertexTypeColor[m_vertexCount];

	////first front face
	//verticesColor[0].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);  // Bottom left.
	//verticesColor[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	//verticesColor[1].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  // Top left.
	//verticesColor[1].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);

	//verticesColor[2].position = XMFLOAT3(1.0f, 1.0f, -1.0f);  // Top right.
	//verticesColor[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	//verticesColor[3].position = XMFLOAT3(1.0f, -1.0f, -1.0f);  // Bottom right.
	//verticesColor[3].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f);

	////second face
	//verticesColor[4].position = XMFLOAT3(1.0f, 1.0f, 1.0f);  
	//verticesColor[4].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);

	//verticesColor[5].position = XMFLOAT3(1.0f, -1.0f, 1.0f);  
	//verticesColor[5].color = XMFLOAT4(1.0f, 0.0f, 1.0f, 0.0f);

	////third face
	//verticesColor[6].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);  
	//verticesColor[6].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	//verticesColor[7].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);  
	//verticesColor[7].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

	// Load the index array with data.

	//not messed up vertices
	////first face
	//indices[0] = 0;  // Bottom left.
	//indices[1] = 1;  // Top left.
	//indices[2] = 2;  // Top right.

	//indices[3] = 0;  // Bottom left.
	//indices[4] = 2;  // Top left.
	//indices[5] = 3;  // Bottom right.

	////second face
	//indices[6] = 3;  // Bottom left.
	//indices[7] = 2;  // Top left.
	//indices[8] = 4;  // Top right.

	//indices[9] = 3; // Bottom left.
	//indices[10] = 4; // Top left.
	//indices[11] = 5; // Bottom right.

	//messed up vertices
	//anti clockwise notation!
	//first face
	//indices[0] = 2;  // Bottom left.
	//indices[1] = 1;  // Top left.
	//indices[2] = 0;  // Top right.

	//indices[3] = 2;  // Bottom left.
	//indices[4] = 0;  // Top left.
	//indices[5] = 3;  // Bottom right.

	////second face
	//indices[6] = 4;  // Bottom left.
	//indices[7] = 2;  // Top left.
	//indices[8] = 3;  // Top right.

	//indices[9] = 4; // Bottom left.
	//indices[10] = 3; // Top left.
	//indices[11] = 5; // Bottom right.

	//////third face
	//indices[12] = 7;
	//indices[13] = 4;
	//indices[14] = 5;

	//indices[15] = 7;
	//indices[16] = 6;
	//indices[17] = 4;

	////fourth face (bottom)
	//indices[18] = 0;
	//indices[19] = 5;
	//indices[20] = 3;

	//indices[21] = 0;
	//indices[22] = 7;
	//indices[23] = 5;

	// Set up the description of the static vertex buffer.
	//ByteWidth (size of the buffer)
	//BindFlags (type of buffer)
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeColor) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data. 
	//meaning that with both the DESCription and resc POINTER you can return a pointer to this buffer 

	//swap for below for "textures"
	//vertexData.pSysMem = vertices;
	vertexData.pSysMem = verticesColor;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		Logger::AppendErrorToLog("vertex buffer failed to create");
		return false;
	}

	//DO THE SAME FOR INDEX
	// Set up the description of the static index buffer.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&IndexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		Logger::AppendErrorToLog("index buffer failed to create");
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] verticesColor;
	verticesColor = 0;

	delete [] indices;
	indices = 0;

	return true;

}

//The ShutdownBuffers function just releases the vertex buffer and index buffer that were created in the InitializeBuffers function. 
void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	return;
}

//RenderBuffers is called from the Render function
//needs to activate the buffers on the GPU to apply shaders
//also defines how to be drawn etc triangles
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	//remember to change for texturing!
	stride = sizeof(VertexTypeColor); 
	offset = 0;
    
	//activating the next two buffers means that GPU has the buffers and shaders can be applied to them
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	// Create the texture object.
	m_Texture = new TextureHandler;
	if(!m_Texture)
	{
		Logger::AppendErrorToLog("m_Texture failed to create in ModelClass::LoadTexture");
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		Logger::AppendErrorToLog("m_Texture failed to Initialize in ModelClass::LoadTexture");
		return false;
	}

	return true;

}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;

}
