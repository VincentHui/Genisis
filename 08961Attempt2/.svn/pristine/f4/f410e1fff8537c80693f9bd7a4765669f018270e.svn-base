#include "EllipsoidSceneObject.h"

EllipsoidSceneObject::EllipsoidSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd)
{
	m_model = new ModelLightingClass(name, quad, pos);
	m_quadInvoker = new QuadInvoker;
	m_dataStruct = new clientDataStruct;
	updateIncrement = 0.0f;
	m_name = name;

	m_model->Initialize(device, "../Engine/data/model.txt", NULL, hwnd, false);
	m_quadInvoker->InitializeVertexShader(device, hwnd, L"../Engine/EllipsoidVS.hlsl", "EllipsoidVertexShader", false);
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/EllipsoidHS.hlsl", "EllipsoidHullShader","hs_5_0");
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/EllipsoidDS.hlsl", "EllipsoidDomainShader","ds_5_0");
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/EllipsoidPS.hlsl", "EllipsoidPixelShader","ps_5_0");
	m_quadInvoker->InitializeCBuffer(device, "matrixBuffer", sizeof(QuadInvoker::MatrixBufferType));
	m_quadInvoker->InitializeCBuffer(device, "tessBuffer", sizeof(QuadInvoker::TessellationBufferType));
	m_quadInvoker->InitializeCBuffer(device, "ellipsoidBuffer", sizeof(EllispoidBuffer));
	m_quadInvoker->InitializeCBuffer(device, "LightBuffer", sizeof(QuadInvoker::LightBufferType));
	m_quadInvoker->InitializeSampler(device, "heightSampler");

	m_model->AddModelTweakVar(64.0f , "TorrusTessFactor", " step=0.01 group=torus");
	m_model->AddModelTweakVar(0.6f , "XLen", " step=0.01 group=ellipsoid");
	m_model->AddModelTweakVar(0.6f , "Ylen", " step=0.01 group=ellipsoid");
	m_model->AddModelTweakVar(0.6f , "ZLen", " step=0.01 group=ellipsoid");

	m_dataStruct->m_dev = device;
	m_dataStruct->m_h = hwnd;
	m_dataStruct->m_q = m_quadInvoker;

	TwAddButton(m_model->getBar() ,"FractionalEven", &BasePartition::FractionalEven, m_dataStruct, "" );
	TwAddButton(m_model->getBar() ,"FractionalOdd", &BasePartition::FractionalOdd, m_dataStruct, "" );
	TwAddButton(m_model->getBar() ,"FractionalInteger", &BasePartition::FractionalInteger, m_dataStruct, "" );

}


EllipsoidSceneObject::~EllipsoidSceneObject(void)
{
	delete m_dataStruct;
	m_dataStruct = 0;
	delete m_model;
	m_model = 0;
	delete m_quadInvoker;
	m_quadInvoker = 0;
}

void EllipsoidSceneObject::renderTheScene(GraphicsContext & p_in)
{
	updateIncrement += p_in.deltaTime * 0.01f;
	auto * invoker = m_quadInvoker;
	auto * Model = m_model;
	ID3D11DeviceContext * tempDevice = p_in.GetD3DContext()->GetDeviceContext();

	EllispoidBuffer *tempTorus = invoker->MapCBuffer<EllispoidBuffer>(tempDevice, "ellipsoidBuffer");
	tempTorus->XLen = Model->TweakVariables["XLen"] ;
	tempTorus->YLen = Model->TweakVariables["Ylen"] + (sin(updateIncrement * 0.1f) * 0.1f);
	tempTorus->ZLen = Model->TweakVariables["ZLen"] + (sin(updateIncrement * 0.2f) * 0.2f);
	tempTorus->padding = 1.0f;
	invoker->UnMapCBuffer(tempDevice, "ellipsoidBuffer");
	tempDevice->DSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["ellipsoidBuffer"]);

	XMMATRIX tempMat;
	Model->SetPos(p_in.c_lightInfo->GetPos());
	Model->ModelMatrix(tempMat);

	QuadInvoker::MatrixBufferType *tempMatBuff = invoker->MapCBuffer<QuadInvoker::MatrixBufferType>(tempDevice , "matrixBuffer");
	tempMatBuff->worldMatrix = XMMatrixTranspose(tempMat);
	tempMatBuff->viewMatrix = XMMatrixTranspose( *p_in.c_view);
	tempMatBuff->projectionMatrix = XMMatrixTranspose(*p_in.c_projection);
	invoker->UnMapCBuffer(tempDevice , "matrixBuffer");
	tempDevice->VSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);
	tempDevice->DSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);

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
	tempLightBuff->specularPower = p_in.c_lightInfo->GetSpecularPower();
	invoker->UnMapCBuffer(tempDevice, "LightBuffer");
	tempDevice->PSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["LightBuffer"]);

	Model->RenderAllBuffers<ModelLightingClass::LightingVertexType> (tempDevice, D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	invoker->RenderShader(tempDevice ,  Model->GetIndexCount());

}