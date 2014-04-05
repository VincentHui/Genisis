#include "AlienSceneObject.h"


AlienSceneObject::AlienSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd)
{
	m_model = new ModelLightingClass(name, quad, pos);
	m_model->SetScale(XMFLOAT3(0.01f, 0.01f, 0.01f));
	m_model->Initialize(device, "../Engine/data/teapot.txt", NULL, hwnd, false);

	for(UINT i = 0; i < swarmSize; i++)
	{
		randNums.push_back(rand() % 10 + 30);
	}

	m_quadInvoker = new QuadInvoker;
	updateIncrement = 0.0f;
	m_model->m_textures["AlienSkin"] = new TextureHandler(device , L"../Engine/data/AlienTexture.dds", hwnd);

	m_quadInvoker->InitializeVertexShader(device, hwnd, L"../Engine/AlienVS.hlsl", "AlienVertexShader", false);
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/AlienPS.hlsl", "AlienPixelShader","ps_5_0");
	m_quadInvoker->InitializeCBuffer(device, "matrixBuffer", sizeof(QuadInvoker::MatrixBufferType));
	m_quadInvoker->InitializeCBuffer(device, "LightBuffer", sizeof(QuadInvoker::LightBufferType));
	m_quadInvoker->InitializeCBuffer(device, "LightBuffer2", sizeof(LightPosBuffer));
	m_quadInvoker->InitializeCBuffer(device, "AnimationBuffer", sizeof(animationBuffer));
	m_quadInvoker->InitializeSampler(device, "heightSampler");

}


AlienSceneObject::~AlienSceneObject(void)
{
	delete m_quadInvoker;
	m_quadInvoker = 0;
	delete m_model;
	m_model = 0;


}

void AlienSceneObject::renderTheScene(GraphicsContext & p_in)
{
	updateIncrement += p_in.deltaTime * 0.01f;
	auto * invoker = m_quadInvoker;
	auto * Model = m_model;
	ID3D11DeviceContext * tempDevice = p_in.GetD3DContext()->GetDeviceContext();

	m_model->SetRot(XMFLOAT3( 0.0f, XM_PI/2, 0.0f));

	animationBuffer * tempAnimBuff = invoker->MapCBuffer<animationBuffer>(tempDevice, "AnimationBuffer");
	tempAnimBuff->time = updateIncrement * 0.1f;
	tempAnimBuff->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);
	invoker->UnMapCBuffer(tempDevice, "AnimationBuffer");
	tempDevice->VSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["AnimationBuffer"]);

	QuadInvoker::LightBufferType *tempLightBuff = invoker->MapCBuffer<QuadInvoker::LightBufferType>(tempDevice, "LightBuffer");
	tempLightBuff->ambientColor = p_in.c_lightInfo->GetAmbientColor();
	tempLightBuff->diffuseColor = p_in.c_lightInfo->GetDiffuseColor();
	tempLightBuff->lightDirection = p_in.c_lightInfo->Getdirection();
	tempLightBuff->specularPower = p_in.c_lightInfo->GetSwitcher();
	invoker->UnMapCBuffer(tempDevice, "LightBuffer");
	tempDevice->PSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["LightBuffer"]);

	ID3D11ShaderResourceView* tempArray2[1];
	tempArray2[0] =  m_model->m_textures["AlienSkin"]->GetTexture();
	tempDevice->PSSetShaderResources(0, 1, tempArray2);

	LightPosBuffer *tempLightBuff2 = invoker->MapCBuffer<LightPosBuffer>(tempDevice, "LightBuffer2");
	tempLightBuff2->LightPos = p_in.c_lightInfo->GetPos();
	tempLightBuff2->rangeMod = p_in.c_lightInfo->GetRange();
	invoker->UnMapCBuffer(tempDevice, "LightBuffer2");
	tempDevice->PSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["LightBuffer2"]);



	for(UINT i = 0; i < swarmSize; i++)
	{
		XMMATRIX orbitMatrix = XMMatrixTranslation( (randNums[i] * 0.1f) + sin(i), (i * 0.1f) + (randNums[i] * 0.1) + (sin(updateIncrement + randNums[i]) * 0.1f), 0.0f) * XMMatrixRotationY((updateIncrement * (randNums[i] * 0.005)) + (XM_PI));
		XMMATRIX tempMat;
		m_model->SetRot(XMFLOAT3(0.0, XM_PI/2, 0.0));
		m_model->ModelMatrix(tempMat);

		QuadInvoker::MatrixBufferType *tempMatBuff = invoker->MapCBuffer<QuadInvoker::MatrixBufferType>(tempDevice , "matrixBuffer");
		tempMatBuff->worldMatrix = XMMatrixTranspose(tempMat * orbitMatrix);
		tempMatBuff->viewMatrix = XMMatrixTranspose( *p_in.c_view);
		tempMatBuff->projectionMatrix = XMMatrixTranspose(*p_in.c_projection);
		invoker->UnMapCBuffer(tempDevice , "matrixBuffer");
		tempDevice->VSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);
		m_model->RenderAllBuffers<ModelLightingClass::LightingVertexType> (tempDevice, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		invoker->RenderShader(tempDevice ,  m_model->GetIndexCount());
	}

}