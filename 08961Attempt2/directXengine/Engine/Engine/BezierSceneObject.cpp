#include "BezierSceneObject.h"


BezierSceneObject::BezierSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd)
{
	m_model = new ModelLightingClass(name, quad, pos);
	m_quadInvoker = new QuadInvoker;
	m_dataStruct = new BasePartition::clientDataStruct;

	m_model->BuildQuadPatch( device);
	updateIncrement = 0.0f;
	m_model->SetScale(XMFLOAT3(0.1f, 0.5f, 0.5f));

	m_quadInvoker->InitializeVertexShader(device, hwnd, L"../Engine/BézierVS.hlsl", "BezierVertexShader", false);
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/BézierHS.hlsl", "BezierHullShader","hs_5_0");
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/BézierDS.hlsl", "BezierDomainShader","ds_5_0");
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/BézierPS.hlsl", "BezierPixelShader","ps_5_0");
	m_quadInvoker->InitializeCBuffer(device, "matrixBuffer", sizeof(QuadInvoker::MatrixBufferType));
	m_quadInvoker->InitializeCBuffer(device, "tessBuffer", sizeof(QuadInvoker::TessellationBufferType));
	m_quadInvoker->InitializeCBuffer(device, "LightBuffer", sizeof(QuadInvoker::LightBufferType));
	m_quadInvoker->InitializeSampler(device, "heightSampler");

	m_model->AddModelTweakVar(20.0f , "TessFactor", " step=0.01 group=Bézier");
	m_model->AddModelTweakVar(8.0f , "AmpIncrement", " step=0.01 group=Bézier");
	m_model->AddModelTweakVar(1.0f , "timeMod", " step=0.001 group=Bézier");
	m_model->AddModel3FloatTweakVar(XMFLOAT3(-15.0f, 0.0f, 15.0f), "modifier1", "step=0.01 group=BézierControlGroup1");
	string tempName = name + "/BézierControlGroup1 group=Bézier opened=false";
	TwDefine(tempName.c_str());

	m_dataStruct->m_dev = device;
	m_dataStruct->m_h = hwnd;
	m_dataStruct->m_q = m_quadInvoker;

	TwAddButton(m_model->getBar() ,"FractionalEven", &BezierSceneObject::FractionalEven, m_dataStruct, "" );
	TwAddButton(m_model->getBar() ,"FractionalOdd", &BezierSceneObject::FractionalOdd, m_dataStruct, "" );
	TwAddButton(m_model->getBar() ,"FractionalInt", &BezierSceneObject::Integer, m_dataStruct, "" );

}


BezierSceneObject::~BezierSceneObject(void)
{
	delete m_quadInvoker;
	m_quadInvoker = 0;
	delete m_model;
	m_model = 0;
	delete m_dataStruct;
	m_dataStruct = 0;
}

void BezierSceneObject::renderTheScene(GraphicsContext & p_in)
{
	auto * invoker = m_quadInvoker;
	auto * Model = m_model;

	updateIncrement += p_in.deltaTime * 0.01f *  m_model->TweakVariables["timeMod"];

	ID3D11DeviceContext * tempDevice = p_in.GetD3DContext()->GetDeviceContext();
	
	XMMATRIX tempMat;
	Model->ModelMatrix(tempMat);
	Model->UpdateBuffers(p_in.c_d3d->GetDeviceContext(), updateBezierPoints(updateIncrement, m_model->TweakVariables["AmpIncrement"]));
	
	XMMATRIX orbitMatrix = XMMatrixTranslation(10.0f , 0.0f, 0.0f) * XMMatrixRotationY(updateIncrement * 0.1f);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	QuadInvoker::MatrixBufferType *tempMatBuff = invoker->MapCBuffer<QuadInvoker::MatrixBufferType>(tempDevice , "matrixBuffer");
	tempMatBuff->worldMatrix = XMMatrixTranspose(tempMat* orbitMatrix);
	tempMatBuff->viewMatrix = XMMatrixTranspose( *p_in.c_view);
	tempMatBuff->projectionMatrix = XMMatrixTranspose(*p_in.c_projection);
	invoker->UnMapCBuffer(tempDevice , "matrixBuffer");
	tempDevice->VSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);
	tempDevice->DSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);

	QuadInvoker::TessellationBufferType *tempTessBuff = invoker->MapCBuffer<QuadInvoker::TessellationBufferType>(tempDevice , "tessBuffer");
	tempTessBuff->cameraPosition = p_in.c_cam->GetPos();
	tempTessBuff->misc = XMFLOAT3();
	tempTessBuff->tessellationAmount = m_model->TweakVariables["TessFactor"];
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

	Model->RenderAllBuffers<ModelLightingClass::LightingVertexType> (tempDevice, D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST);
	invoker->RenderShader(tempDevice ,  Model->GetIndexCount());
}