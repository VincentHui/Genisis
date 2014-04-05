////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ModelLightingClass.h"



ModelLightingClass::ModelLightingClass(string barName, bool p_quad, const XMFLOAT3 & postion):Entity(),
	Position(postion), Rotation(0.0f, 0.0f, 0.0f), Scale(1.0f, 1.0f, 1.0f), Quaternion(XMQuaternionIdentity())
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_model = 0;


	m_bar = TwNewBar(barName.c_str());

	string define = barName + " iconified=true  alpha=200";
	string group = barName + "/Rotation opened = false";
	TwDefine(define.c_str());

	TwAddVarRW(m_bar, "XRotation", TW_TYPE_FLOAT, &Rotation.x, " step=0.01 group=Rotation");
	TwAddVarRW(m_bar, "YRotation", TW_TYPE_FLOAT, &Rotation.y, " step=0.01 group=Rotation");
	TwAddVarRW(m_bar, "ZRotation", TW_TYPE_FLOAT, &Rotation.z, "min=0 max=6.28 step=0.01 group=Rotation");
	TwDefine(group.c_str());

	TwAddVarRW(m_bar, "XPosition", TW_TYPE_FLOAT, &Position.x, "  step=0.1 group=Position");
	TwAddVarRW(m_bar, "YPosition", TW_TYPE_FLOAT, &Position.y, "  step=0.1 group=Position");
	TwAddVarRW(m_bar, "ZPosition", TW_TYPE_FLOAT, &Position.z, "  step=0.1 group=Position");
	group = barName + "/Position opened = false";
	TwDefine(group.c_str());

	TwAddVarRW(m_bar, "XScale", TW_TYPE_FLOAT, &Scale.x, "  step=0.01 group=Scale");
	TwAddVarRW(m_bar, "YScale", TW_TYPE_FLOAT, &Scale.y, "  step=0.01 group=Scale");
	TwAddVarRW(m_bar, "ZScale", TW_TYPE_FLOAT, &Scale.z, "  step=0.01 group=Scale");
	group = barName + "/Scale opened = false";
	TwDefine(group.c_str());
}

ModelLightingClass::ModelLightingClass(const ModelLightingClass & other)
{
}

ModelLightingClass::~ModelLightingClass()
{
	for ( auto local_it = m_textures.begin(); local_it!= m_textures.end(); ++local_it )
	{
		if(local_it->second)
		{
			local_it->second->shutdown();
			delete local_it->second;
			local_it->second = 0;
		}
	}
}

void ModelLightingClass::ModelMatrix(XMMATRIX& p_out)
{
	XMMATRIX trans = XMMatrixTranslation(Position.x, Position.y, Position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z );
	XMMATRIX scale = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	p_out = scale * rot * trans;
}

const XMMATRIX & ModelLightingClass::ModelMatrix()
{
	XMMATRIX trans = XMMatrixTranslation(Position.x, Position.y, Position.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z );
	XMMATRIX scale = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	model_ModelWorld = scale * rot * trans;
	return model_ModelWorld;
}

bool ModelLightingClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR** textureFilename, HWND hwnd, bool invert)
{
	bool result;
	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device, invert);
	if(!result)
	{
		return false;
	}


	return true;
}


void ModelLightingClass::Shutdown()
{

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


int ModelLightingClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelLightingClass::InitializeBuffers(ID3D11Device* device, bool DrawInverted)
{
	LightingVertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	invert = DrawInverted;

	// Create the vertex array.
	vertices = new LightingVertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}


	// Load the vertex array and index array with data.
	for(i=0; i<m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);

		if(!invert)
		{
			//non inverted cube
			vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
			indices[i] = i;
		}
		else
		{
			//inverted cube
			vertices[i].normal = XMFLOAT3(-m_model[i].nx, -m_model[i].ny, -m_model[i].nz);
			indices[i] = m_vertexCount - i -1;
		}

	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(LightingVertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
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
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void ModelLightingClass::ShutdownBuffers()
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





bool ModelLightingClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(int i=0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

bool ModelLightingClass::LoadBlenderOBJ(char* file)
{
	return true;
}

void ModelLightingClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	return;
}

void ModelLightingClass::update()
{
	//rot += rotSpeed;
	//if(rot > 6.28f)
	//	rot = 0.0f;

	//model_ModelWorld = XMMatrixIdentity();

	//model_Translation = XMMatrixTranslation(0.0f,0.0f, rot);
	//XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//model_Rotation = XMMatrixRotationAxis(rotaxis, rot);

	//model_ModelWorld = model_Rotation;
}