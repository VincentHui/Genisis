#include "HLSL_invoker.h"


HLSL_invoker::HLSL_invoker(void)
{
	//set all pointers to null
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

HLSL_invoker::~HLSL_invoker(void)
{
}

bool HLSL_invoker::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"../Engine/color.vs", L"../Engine/color.ps");
	if(!result)
	{
		Logger::AppendErrorToLog("InitializeShader in HLSL_invoker failed");
		return false;
	}
	return true;
}

void HLSL_invoker::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


//Once the parameters are set it calls RenderShader to draw the green triangle using the HLSL shader
bool HLSL_invoker::Render(ID3D11DeviceContext* deviceContext, int indexCount,  XMFLOAT4X4  worldMatrix, 
			       XMFLOAT4X4  viewMatrix,  XMFLOAT4X4  projectionMatrix)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		Logger::AppendErrorToLog("SetShaderParameters in HLSL_invoker failed");
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

//VERY important function. Loads the shader files
//MAKE it match the VertexType in the modelclass.h and color.vs file
bool HLSL_invoker::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//compile the vertex shader!
	
	result = D3DCompileFromFile (vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
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


	//// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
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

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
					   vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		Logger::AppendErrorToLog("device->CreateInputLayout failed in HLSL_invoker::InitializeShader");
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

	return true;

}

//releases the four interfaces that were setup in the InitializeShader function
void HLSL_invoker::ShutdownShader()
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

	return;
}

void HLSL_invoker::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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
// this function is called to send them into the vertex shader during the Render call in HLSL_invoker. 
bool HLSL_invoker::SetShaderParameters(ID3D11DeviceContext* p_DeviceContext,  XMFLOAT4X4 p_worldMatrix,  XMFLOAT4X4 p_viewMatrix,  XMFLOAT4X4 p_projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	//transpose the matrices for directx
	//Fortunately, most of the processing in Direct3D is performed in the video card. 
	//An occasional, unnecessary 64 byte copy because of a poor design decision from Microsoft should be imperceptible.
	//http://stackoverflow.com/questions/15245378/directxxmmatrix-declspecalign16-wont-be-aligned
	//BULLSHIT!!!!


	worldMatrix = XMLoadFloat4x4(&p_worldMatrix);
	viewMatrix = XMLoadFloat4x4(&p_viewMatrix);
	projectionMatrix = XMLoadFloat4x4(&p_projectionMatrix);
	//deprected version didn't have to cope with the bullshit above this
	//XMMATRIX transMatrix = XMMatrixTranslation(0,-2,0);
	//worldMatrix *= transMatrix;

	worldMatrix = XMMatrixTranspose(worldMatrix); 
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = p_DeviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		Logger::AppendErrorToLog("p_DeviceContext couldn't Map, at HLSL_invoker");
		return false;
	}


	// Copy the matrices into the constant buffer.
	//more converting BULLSHIT!
	XMFLOAT4X4 _4X4worldMatrix;
	XMStoreFloat4x4(&_4X4worldMatrix, worldMatrix);
	XMFLOAT4X4 _4X4viewMatrix;
	XMStoreFloat4x4(&_4X4viewMatrix, viewMatrix);
	XMFLOAT4X4 _4X4projectionMatrix;
	XMStoreFloat4x4(&_4X4projectionMatrix, projectionMatrix);
	//end of bullshit


	//Lock the m_matrixBuffer, set the new matrices inside it, and then unlock it. 
	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	
	dataPtr->worldMatrix = _4X4worldMatrix;
	dataPtr->viewMatrix = _4X4viewMatrix;
	dataPtr->projectionMatrix = _4X4projectionMatrix;

	// Unlock the constant buffer.
	p_DeviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	p_DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

void HLSL_invoker::RenderShader(ID3D11DeviceContext* p_deviceContext, int p_indexCount)
{
	// Set the vertex input layout.
	// This lets the GPU know the format of the data in the vertex buffer
	p_deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	p_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	p_deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Render the triangle.
	p_deviceContext->DrawIndexed(p_indexCount, 0, 0);
	

	return;
}
