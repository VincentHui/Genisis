#include "QuadInvoker.h"



QuadInvoker::QuadInvoker(void):Entity()
{
	//set all pointers to null
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_hullShader = 0;
	m_domainShader = 0;
	m_sampleState = 0;
	m_geometryShader = 0;
}

QuadInvoker::~QuadInvoker(void)
{
	this->ShutdownShader();


}


bool  QuadInvoker::InitializeVertexShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, LPCSTR vsEntry, bool simpleQuad)
{
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* errorMessage;
	unsigned int numElements;

	HRESULT result = D3DCompileFromFile (vsFilename, NULL, NULL, vsEntry, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
		&vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
			Logger::AppendErrorToLog("vertex shader compile error!");
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"missing shader file", MB_OK);
			Logger::AppendErrorToLog("missing shader file");
		}
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	if(simpleQuad)
	{
		D3D11_INPUT_ELEMENT_DESC polygonLayout2[2];
		polygonLayout2[0].SemanticName = "POSITION";
		polygonLayout2[0].SemanticIndex = 0;
		polygonLayout2[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout2[0].InputSlot = 0;
		polygonLayout2[0].AlignedByteOffset = 0;
		polygonLayout2[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout2[0].InstanceDataStepRate = 0;

		polygonLayout2[1].SemanticName = "TEXCOORD";
		polygonLayout2[1].SemanticIndex = 0;
		polygonLayout2[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout2[1].InputSlot = 0;
		polygonLayout2[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout2[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout2[1].InstanceDataStepRate = 0;

			// Get a count of the elements in the layout.
		numElements = sizeof(polygonLayout2) / sizeof(polygonLayout2[0]);
		//numElements = 3;
		// Create the vertex input layout.
		result = device->CreateInputLayout(polygonLayout2, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
		if(FAILED(result))
		{
			return false;
		}

		// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
		vertexShaderBuffer->Release();
		vertexShaderBuffer = 0;
		
		return true;
	}
	
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	//numElements = 3;
	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	return true;
}

bool QuadInvoker::InitializeShaderProgram(ID3D11Device* device, HWND hwnd, WCHAR* hsFilename, LPCSTR Entry, LPCSTR ShaderModel)
{
	ID3D10Blob* ShaderBuffer;
	ID3D10Blob* errorMessage;

	// Compile the shader code.
	HRESULT result = D3DCompileFromFile(hsFilename, NULL, NULL, Entry, ShaderModel, D3D10_SHADER_ENABLE_STRICTNESS, 0,  
		&ShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, hsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, hsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	string temp = ShaderModel;
	int test = temp.find("hs");
	if(temp.find("hs") != -1)
	{
		m_hullShader = 0;
		result = device->CreateHullShader(ShaderBuffer->GetBufferPointer(), ShaderBuffer->GetBufferSize(), NULL, &m_hullShader);
		if(FAILED(result))
		{
			MessageBox(hwnd, hsFilename, L"Hull shader failed to load", MB_OK);
			return false;
		}
		ShaderBuffer->Release();
		ShaderBuffer = 0;
		return true;
	}
	if(temp.find("ds") != -1)
	{
		m_domainShader = 0;
		result = device->CreateDomainShader(ShaderBuffer->GetBufferPointer(), ShaderBuffer->GetBufferSize(), NULL, &m_domainShader);
		if(FAILED(result))
		{
			MessageBox(hwnd, hsFilename, L"Domain shader failed to load", MB_OK);
			return false;
		}
		ShaderBuffer->Release();
		ShaderBuffer = 0;
		return true;
	}
	if(temp.find("ps") != -1)
	{
		m_pixelShader = 0;
		result = device->CreatePixelShader(ShaderBuffer->GetBufferPointer(), ShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
		if(FAILED(result))
		{
			MessageBox(hwnd, hsFilename, L"Pixel shader failed to load", MB_OK);
			return false;
		}
		ShaderBuffer->Release();
		ShaderBuffer = 0;
		return true;
	}
	if(temp.find("gs") != -1)
	{
		m_geometryShader = 0;
		result = device->CreateGeometryShader(ShaderBuffer->GetBufferPointer(), ShaderBuffer->GetBufferSize(), NULL, &m_geometryShader);
		if(FAILED(result))
		{
			MessageBox(hwnd, hsFilename, L"geometry shader failed to load", MB_OK);
			return false;
		}
		ShaderBuffer->Release();
		ShaderBuffer = 0;
		return true;
	}

	return false;
}

bool QuadInvoker::InitializeCBuffer(ID3D11Device* device, char* bufferName,  UINT sizeOfStruct)
{
	D3D11_BUFFER_DESC Buffer;
	Buffer.Usage = D3D11_USAGE_DYNAMIC;
	Buffer.ByteWidth = sizeOfStruct;
	Buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Buffer.MiscFlags = 0;
	Buffer.StructureByteStride = 0;

	bufferMap[bufferName] = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HRESULT result = device->CreateBuffer(&Buffer, NULL, &bufferMap[bufferName]);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

unordered_map<string, ID3D11Buffer*> QuadInvoker::GetBufferMap()
{
	return bufferMap;
}

bool QuadInvoker::InitializeSampler(ID3D11Device* device, char* SamplerName)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	samplerMap[SamplerName] = 0;

	// Create the texture sampler state.
	HRESULT result = device->CreateSamplerState(&samplerDesc, &samplerMap[SamplerName]);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

//releases the four interfaces that were setup in the InitializeShader function
void QuadInvoker::ShutdownShader()
{
	//std::unordered_map<std::string, ID3D11Buffer*>::const_iterator got = bufferMap.find("matrixBuffer");
	for ( auto local_it = bufferMap.begin(); local_it!= bufferMap.end(); ++local_it )
	{
		if(local_it->second)
		{
			local_it->second->Release();
			local_it->second = 0;
		}
	}


	for ( auto local_it = samplerMap.begin(); local_it!= samplerMap.end(); ++local_it )
	{
		if(local_it->second)
		{	
			local_it->second->Release();
			local_it->second = 0;
		}
	}

	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	// Release the domain shader.
	if(m_domainShader)
	{
		m_domainShader->Release();
		m_domainShader = 0;
	}

	// Release the hull shader.
	if(m_hullShader)
	{
		m_hullShader->Release();
		m_hullShader = 0;
	}

	if(m_geometryShader)
	{
		m_geometryShader->Release();
		m_geometryShader = 0;
	}
	return;
}

void QuadInvoker::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();


	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();
	errorMessage->Release();
	errorMessage = 0;
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


bool QuadInvoker::MapMatrixBuffer(ID3D11DeviceContext* p_DeviceContext, char* bufferName, MatrixBufferType * p_matBuffer)
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
	p_matBuffer = (MatrixBufferType*)m_mappedResource.pData;

	return true;
}

bool QuadInvoker::UnMapCBuffer(ID3D11DeviceContext* p_DeviceContext, string bufferName)
{
	std::unordered_map<std::string, ID3D11Buffer*>::const_iterator got = bufferMap.find (bufferName);
	if(got == bufferMap.end())
	{
		return false;
	}
	p_DeviceContext->Unmap(got->second, 0);
	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;

	return true;
}

void QuadInvoker::RenderShader(ID3D11DeviceContext* p_deviceContext, int p_indexCount)
{
	// Set the vertex input layout.
	// This lets the GPU know the format of the data in the vertex buffer
	p_deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	p_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	p_deviceContext->HSSetShader(m_hullShader, NULL, 0);
	p_deviceContext->GSSetShader(m_geometryShader, NULL, 0);
	p_deviceContext->DSSetShader(m_domainShader, NULL, 0);
	p_deviceContext->DSSetSamplers(0, 1, &samplerMap["heightSampler"]);

	p_deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	p_deviceContext->PSSetSamplers(0, 1, & samplerMap["heightSampler"]);

	// Render the triangle.
	p_deviceContext->DrawIndexed(p_indexCount, 0, 0);

	return;
}
