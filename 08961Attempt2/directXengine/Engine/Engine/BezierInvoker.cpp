#include "BezierInvoker.h"



BezierInvoker::BezierInvoker(void)
{
	//set all pointers to null
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_hullShader = 0;
	m_domainShader = 0;
	m_tessellationBuffer = 0;
	m_TesDisplacementBuffer = 0;
	m_sampleState = 0;
	m_cameraBuffer = 0;
	m_lightBuffer = 0;
}

BezierInvoker::~BezierInvoker(void)
{
}

bool BezierInvoker::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	// Initialize the vertex and pixel shaders.
	result = InitializBezierShader(device, hwnd, L"../Engine/BézierVS.hlsl", L"../Engine/BézierHS.hlsl", L"../Engine/BézierDS.hlsl", L"../Engine/BézierPS.hlsl");
	if(!result)
	{
		Logger::AppendErrorToLog("InitializeShader in BezierInvoker failed");
		return false;
	}
	return true;
}

void BezierInvoker::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


//Once the parameters are set it calls RenderShader to draw the green triangle using the HLSL shader
bool BezierInvoker::Render(ID3D11DeviceContext* deviceContext, int indexCount,  const XMMATRIX&  worldMatrix, 
			       const XMMATRIX&  viewMatrix,  const XMMATRIX&  projectionMatrix, 
				   float p_tessFactor,
				   float p_tessScale, float p_tessBias, ID3D11ShaderResourceView** p_textureArray,
				   const XMFLOAT3 & p_camPos,
				   const XMFLOAT4 & p_tess_misc,
				   const LightInfo& LightInfo)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, p_tessFactor, p_tessBias, p_tessScale, p_textureArray, p_camPos, p_tess_misc, LightInfo);
	if(!result)
	{
		Logger::AppendErrorToLog("SetShaderParameters in BezierInvoker failed");
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

	bool BezierInvoker::InitializBezierShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC tessellationBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC TesDisplacementBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	hullShaderBuffer = 0;
	domainShaderBuffer = 0;

	//compile the vertex shader!
	
	result = D3DCompileFromFile (vsFilename, NULL, NULL, "BezierVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
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

		return false;
	}

	// Compile the hull shader code.
	result = D3DCompileFromFile(hsFilename, NULL, NULL, "BezierHullShader", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,  
				       &hullShaderBuffer, &errorMessage);
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

		// Compile the domain shader code.
	result = D3DCompileFromFile(dsFilename, NULL, NULL, "BezierDomainShader", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,  
				       &domainShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, dsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, dsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}


	//// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "BezierPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
				       &pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
			Logger::AppendErrorToLog("pixel shader compile error!");
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
			Logger::AppendErrorToLog("missing shader file");
		}

		return false;
	}

	//Once the vertex shader + pixel shader code has compiled into buffers 
	//use those buffers to create shader objects
	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the hull shader from the buffer.
	result = device->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &m_hullShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the domain shader from the buffer.
	result = device->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &m_domainShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	//pay attention to semantics!
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

	// add a third element for the normal vector that will be used for lighting in polygon layout
	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	//constant buffer = cbuffer
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}


	// Create a texture sampler state description.
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

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic tessellation constant buffer that is in the hull shader.
	tessellationBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tessellationBufferDesc.ByteWidth = sizeof(TessellationBufferType);
	tessellationBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tessellationBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tessellationBufferDesc.MiscFlags = 0;
	tessellationBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the hull shader constant buffer from within this class.
	result = device->CreateBuffer(&tessellationBufferDesc, NULL, &m_tessellationBuffer);
	if(FAILED(result))
	{
		return false;
	}

	
	// Setup the description of the dynamic tessellation constant buffer that is in the hull shader.
	TesDisplacementBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	TesDisplacementBufferDesc.ByteWidth = sizeof(TesDisplacementBufferType);
	TesDisplacementBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	TesDisplacementBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TesDisplacementBufferDesc.MiscFlags = 0;
	TesDisplacementBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the hull shader constant buffer from within this class.
	result = device->CreateBuffer(&TesDisplacementBufferDesc, NULL, &m_TesDisplacementBuffer);
	if(FAILED(result))
	{
		return false;
	}



	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if(FAILED(result))
	{
		return false;
	}

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

//releases the four interfaces that were setup in the InitializeShader function
void BezierInvoker::ShutdownShader()
{
	// Release the matrix constant buffer.
	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
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

	// Release the tessellation constant buffer.
	if(m_tessellationBuffer)
	{
		m_tessellationBuffer->Release();
		m_tessellationBuffer = 0;
	}

	if( m_TesDisplacementBuffer)
	{
		m_TesDisplacementBuffer->Release();
		m_TesDisplacementBuffer = 0;
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

	if(m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}
	return;
}

void BezierInvoker::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

// SetShaderVariables makes setting the global variables in the shader easier. The matrices are created in GraphicsClass
// this function is called to send them into the vertex shader during the Render call in BezierInvoker. 
bool BezierInvoker::SetShaderParameters(ID3D11DeviceContext* p_DeviceContext,  const XMMATRIX& p_worldMatrix,  const XMMATRIX& p_viewMatrix,  const XMMATRIX& p_projectionMatrix, 
										   float p_tessfactor, float p_tessBias, float p_tessScale
										   , ID3D11ShaderResourceView** p_textureArray, const XMFLOAT3 & p_camPos, const XMFLOAT4 & p_tess_misc
										   ,const LightInfo& LightInfo)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	TessellationBufferType* dataPtr2;
	TesDisplacementBufferType* dataPtr3;
	CameraBufferType* dataPtr4;
	LightBufferType* dataPtr5;

	unsigned int bufferNumber;
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	//transpose the matrices for directx
	//Fortunately, most of the processing in Direct3D is performed in the video card. 
	//An occasional, unnecessary 64 byte copy because of a poor design decision from Microsoft should be imperceptible.
	//http://stackoverflow.com/questions/15245378/directxxmmatrix-declspecalign16-wont-be-aligned
	//BULLSHIT!!!!

	//worldMatrix = XMLoadFloat4x4(&p_worldMatrix);
	//viewMatrix = XMLoadFloat4x4(&p_viewMatrix);
	//projectionMatrix = XMLoadFloat4x4(&p_projectionMatrix);
	//deprected version didn't have to cope with the bullshit above this

	worldMatrix = XMMatrixTranspose(p_worldMatrix); 
	viewMatrix = XMMatrixTranspose(p_viewMatrix);
	projectionMatrix = XMMatrixTranspose(p_projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = p_DeviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		Logger::AppendErrorToLog("p_DeviceContext couldn't Map, at BezierInvoker");
		return false;
	}


	// Copy the matrices into the constant buffer.
	//more converting BULLSHIT!
	//XMFLOAT4X4 _4X4worldMatrix;
	//XMStoreFloat4x4(&_4X4worldMatrix, worldMatrix);
	//XMFLOAT4X4 _4X4viewMatrix;
	//XMStoreFloat4x4(&_4X4viewMatrix, viewMatrix);
	//XMFLOAT4X4 _4X4projectionMatrix;
	//XMStoreFloat4x4(&_4X4projectionMatrix, projectionMatrix);
	//end of bullshit

	//MatrixBufferType* temp, *temp2 = new MatrixBufferType;
	//temp2->worldMatrix = worldMatrix;
	//temp2->viewMatrix = viewMatrix;
	//temp2->projectionMatrix = projectionMatrix; 


	 
	
	//Lock the m_matrixBuffer, set the new matrices inside it, and then unlock it. 
	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->worldMatrix = worldMatrix;
	dataPtr->viewMatrix = viewMatrix;
	dataPtr->projectionMatrix = projectionMatrix; 
	//BufferBase * test = dataPtr;

	//dataPtr->MapPtr(temp2);


	//dataPtr(temp2);

	// Unlock the constant buffer.
	p_DeviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	//// Finanly set the constant buffer in the vertex shader with the updated values.
	p_DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	p_DeviceContext->DSSetShaderResources(0, 1, &p_textureArray[0]);
	p_DeviceContext->PSSetShaderResources(0, 1, &p_textureArray[1]);
		// Finally set the matrix constant buffer in the domain shader with the updated values.
	p_DeviceContext->DSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

















	result = p_DeviceContext->Map(m_TesDisplacementBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		Logger::AppendErrorToLog("p_DeviceContext couldn't Map, at BezierInvoker");
		return false;
	}


	//TesDisplacementBufferType * temp = new te;
	//temp.scale = p_tessScale;
	//temp.bias = p_tessBias;
	//temp.padding = XMFLOAT2(0.0f, 0.0f);

	// Get a pointer to the data in the tessellation constant buffer.
	dataPtr3 = (TesDisplacementBufferType*)mappedResource.pData;
	//dataPtr3 = &temp;
	// Copy the tessellation data into the constant buffer.
	dataPtr3->scale = p_tessScale;
	dataPtr3->bias = p_tessBias;
	dataPtr3->padding = XMFLOAT2(0.0f, 0.0f);

	// Unlock the tessellation constant buffer.
	p_DeviceContext->Unmap(m_TesDisplacementBuffer, 0);

	// Set the position of the tessellation constant buffer in the hull shader.
	bufferNumber = 1;

	// Now set the tessellation constant buffer in the hull shader with the updated values.
	p_DeviceContext->DSSetConstantBuffers(bufferNumber, 1, &m_TesDisplacementBuffer);












	// Lock the tessellation constant buffer so it can be written to.
	result = p_DeviceContext->Map(m_tessellationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the tessellation constant buffer.
	dataPtr2 = (TessellationBufferType*)mappedResource.pData;

	// Copy the tessellation data into the constant buffer.
	dataPtr2->tessellationAmount = p_tessfactor;
	dataPtr2->cameraPosition = p_camPos;
	dataPtr2->misc = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Unlock the tessellation constant buffer.
	p_DeviceContext->Unmap(m_tessellationBuffer, 0);

	// Set the position of the tessellation constant buffer in the hull shader.
	bufferNumber = 0;

	// Now set the tessellation constant buffer in the hull shader with the updated values.
	p_DeviceContext->HSSetConstantBuffers(bufferNumber, 1, &m_tessellationBuffer);









		// Lock the camera constant buffer so it can be written to.
	result = p_DeviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr4 = (CameraBufferType*)mappedResource.pData;

	// Copy the camera position into the constant buffer.
	dataPtr4->cameraPosition = p_camPos;
	dataPtr4->padding = 0.0f;
	

	// Unlock the camera constant buffer.
	p_DeviceContext->Unmap(m_cameraBuffer, 0);

	// Set the position of the camera constant buffer in the vertex shader.
	//this one is second  buffer in the vertex shadder, first being the matrix buffer
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	p_DeviceContext->HSSetConstantBuffers(2, 1, &m_cameraBuffer);
	p_DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);











	result = p_DeviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	dataPtr5 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr5->ambientColor = LightInfo.GetAmbientColor();
	dataPtr5->diffuseColor =  LightInfo.GetDiffuseColor();
	dataPtr5->lightDirection = LightInfo.Getdirection();
	dataPtr5->specularColor = LightInfo.GetSpecularColor();
	dataPtr5->specularPower = LightInfo.GetSpecularPower();

	// Unlock the constant buffer.
	p_DeviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	p_DeviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	
	return true;
}

void BezierInvoker::RenderShader(ID3D11DeviceContext* p_deviceContext, int p_indexCount)
{
	// Set the vertex input layout.
	// This lets the GPU know the format of the data in the vertex buffer
	p_deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	p_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	p_deviceContext->HSSetShader(m_hullShader, NULL, 0);

	p_deviceContext->DSSetShader(m_domainShader, NULL, 0);
	p_deviceContext->DSSetSamplers(0, 1, &m_sampleState);

	p_deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	p_deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	p_deviceContext->DrawIndexed(p_indexCount, 0, 0);

	return;
}
