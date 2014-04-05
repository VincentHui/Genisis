#include "TorrusSceneObject.h"


TorrusSceneObject::TorrusSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd)
{
	m_model = new ModelLightingClass(name, quad, pos);
	m_quadInvoker = new QuadInvoker;
	m_dataStruct = new clientDataStruct;
	updateIncrement = 0;
	m_name = name;

	m_model->Initialize(device, "../Engine/data/model.txt", NULL, hwnd, false);
	m_quadInvoker->InitializeVertexShader(device, hwnd, L"../Engine/TorusVS.hlsl", "TorusVertexShader", false);
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/TorusHS.hlsl", "TorusHullShader","hs_5_0");
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/TorusDS.hlsl", "TorusDomainShader","ds_5_0");
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/TorusPS.hlsl", "TorusPixelShader","ps_5_0");
	m_quadInvoker->InitializeCBuffer(device, "matrixBuffer", sizeof(QuadInvoker::MatrixBufferType));
	m_quadInvoker->InitializeCBuffer(device, "tessBuffer", sizeof(QuadInvoker::TessellationBufferType));
	m_quadInvoker->InitializeCBuffer(device, "torusBuffer", sizeof(TorusBuffer));
	m_quadInvoker->InitializeCBuffer(device, "LightBuffer", sizeof(QuadInvoker::LightBufferType));
	m_quadInvoker->InitializeCBuffer(device, "LightBuffer2", sizeof(LightPosBuffer));
	m_quadInvoker->InitializeSampler(device, "heightSampler");

	m_model->AddModelTweakVar(64.0f , "TorrusTessFactor", " step=0.01 group=torus");
	m_model->AddModelTweakVar(10.0f , "OuterRadius", " step=0.01 group=torus");
	m_model->AddModelTweakVar(1.0f , "TubeRadius", " step=0.01 group=torus");

	m_model->m_textures["TorusSkin"] = new TextureHandler(device , L"../Engine/data/brick.dds", hwnd);

	m_dataStruct->m_dev = device;
	m_dataStruct->m_h = hwnd;
	m_dataStruct->m_q = m_quadInvoker;

	TwAddButton(m_model->getBar() ,"FractionalEven", &BasePartition::FractionalEven, m_dataStruct, "" );
	TwAddButton(m_model->getBar() ,"FractionalOdd", &BasePartition::FractionalOdd, m_dataStruct, "" );
	TwAddButton(m_model->getBar() ,"FractionalInteger", &BasePartition::FractionalInteger, m_dataStruct, "" );
}



TorrusSceneObject::~TorrusSceneObject(void)
{
	delete m_quadInvoker;
	m_quadInvoker = 0;
	delete m_model;
	m_model = 0;
	delete m_dataStruct;
	m_dataStruct = 0;
}

void TorrusSceneObject::renderTheScene( GraphicsContext & p_in)
{
	updateIncrement += p_in.deltaTime * 0.01f;

	auto * invoker = m_quadInvoker;
	auto * Model = m_model;
	ID3D11DeviceContext * tempDevice = p_in.GetD3DContext()->GetDeviceContext();

	QuadInvoker::TessellationBufferType *tempTessBuff = invoker->MapCBuffer<QuadInvoker::TessellationBufferType>(tempDevice , "tessBuffer");
	tempTessBuff->cameraPosition = p_in.c_cam->GetPos();
	tempTessBuff->misc = XMFLOAT3();
	tempTessBuff->tessellationAmount = m_model->TweakVariables["TorrusTessFactor"];
	tempTessBuff->LODModifier = 0;
	invoker->UnMapCBuffer(tempDevice , "tessBuffer");
	tempDevice->HSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["tessBuffer"]);

	QuadInvoker::LightBufferType *tempLightBuff = invoker->MapCBuffer<QuadInvoker::LightBufferType>(tempDevice, "LightBuffer");
	tempLightBuff->ambientColor = p_in.c_lightInfo->GetAmbientColor();
	tempLightBuff->diffuseColor = p_in.c_lightInfo->GetDiffuseColor();
	tempLightBuff->lightDirection = p_in.c_lightInfo->Getdirection();
	tempLightBuff->specularPower = p_in.c_lightInfo->GetSwitcher();
	invoker->UnMapCBuffer(tempDevice, "LightBuffer");
	tempDevice->PSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["LightBuffer"]);

	LightPosBuffer *tempLightBuff2 = invoker->MapCBuffer<LightPosBuffer>(tempDevice, "LightBuffer2");
	tempLightBuff2->LightPos = p_in.c_lightInfo->GetPos();
	tempLightBuff2->rangeMod = p_in.c_lightInfo->GetRange();
	invoker->UnMapCBuffer(tempDevice, "LightBuffer2");
	tempDevice->PSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["LightBuffer2"]);

	ID3D11ShaderResourceView* tempArray2[1];
	tempArray2[0] =  m_model->m_textures["TorusSkin"]->GetTexture();
	tempDevice->PSSetShaderResources(0, 1, tempArray2);

	//TorusBuffer *tempTorus = invoker->MapCBuffer<TorusBuffer>(tempDevice, "torusBuffer");
	//tempTorus->innerRadius = Model->TweakVariables["TubeRadius"];
	//tempTorus->outerRadius  =  Model->TweakVariables["OuterRadius"];
	//tempTorus->padding = XMFLOAT2();
	//invoker->UnMapCBuffer(tempDevice, "torusBuffer");
	//tempDevice->DSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["torusBuffer"]);





	//rotMat *= XMMatrixScaling(0.5f, 0.5f, 0.5f);
	//XMMatrixScaling()
	//rotMat = XMMatrixRotationY(updateIncrement * 0.005f);














	XMMATRIX tempMat, total;
	//Model->SetPos(p_in.c_lightInfo->GetPos());
	Model->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	Model->SetRot(XMFLOAT3( updateIncrement * 0.01f ,updateIncrement * 0.01f, 0.0f));
	Model->ModelMatrix(tempMat);
	

	TorusBuffer *tempTorus = invoker->MapCBuffer<TorusBuffer>(tempDevice, "torusBuffer");
	tempTorus->innerRadius = Model->TweakVariables["TubeRadius"] * 0.2f;
	tempTorus->outerRadius  =  Model->TweakVariables["OuterRadius"] * 1.4f;
	tempTorus->padding = XMFLOAT2();
	invoker->UnMapCBuffer(tempDevice, "torusBuffer");
	tempDevice->DSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["torusBuffer"]);


	QuadInvoker::MatrixBufferType *tempMatBuff = invoker->MapCBuffer<QuadInvoker::MatrixBufferType>(tempDevice , "matrixBuffer");
	//tempMatBuff->worldMatrix = XMMatrixTranspose(tempMat * *p_in.c_world);
	tempMatBuff->worldMatrix = XMMatrixTranspose(tempMat);
	tempMatBuff->viewMatrix = XMMatrixTranspose( *p_in.c_view);
	tempMatBuff->projectionMatrix = XMMatrixTranspose(*p_in.c_projection);
	invoker->UnMapCBuffer(tempDevice , "matrixBuffer");
	tempDevice->VSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);
	tempDevice->DSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);

	Model->RenderAllBuffers<ModelLightingClass::LightingVertexType> (tempDevice, D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	invoker->RenderShader(tempDevice ,  Model->GetIndexCount());

	


	tempTorus = invoker->MapCBuffer<TorusBuffer>(tempDevice, "torusBuffer");
	tempTorus->innerRadius = Model->TweakVariables["TubeRadius"];
	tempTorus->outerRadius  =  Model->TweakVariables["OuterRadius"];
	tempTorus->padding = XMFLOAT2();
	invoker->UnMapCBuffer(tempDevice, "torusBuffer");
	tempDevice->DSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["torusBuffer"]);

	Model->SetPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	Model->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	Model->SetRot(XMFLOAT3(0.0f, 0.0f, 0.0f));
	Model->ModelMatrix(tempMat);

	tempMatBuff = invoker->MapCBuffer<QuadInvoker::MatrixBufferType>(tempDevice , "matrixBuffer");
	tempMatBuff->worldMatrix = XMMatrixTranspose(tempMat);
	tempMatBuff->viewMatrix = XMMatrixTranspose( *p_in.c_view);
	tempMatBuff->projectionMatrix = XMMatrixTranspose(*p_in.c_projection);
	invoker->UnMapCBuffer(tempDevice , "matrixBuffer");
	tempDevice->VSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);
	tempDevice->DSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);

	Model->RenderAllBuffers<ModelLightingClass::LightingVertexType> (tempDevice, D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	invoker->RenderShader(tempDevice ,  Model->GetIndexCount());

}