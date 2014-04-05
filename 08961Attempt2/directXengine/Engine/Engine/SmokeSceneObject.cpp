#include "SmokeSceneObject.h"


SmokeSceneObject::SmokeSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd)
{
	m_model = new ModelLightingClass(name, quad, pos);
	m_quadInvoker = new QuadInvoker;

	m_model->Initialize(device, "../Engine/data/sphere2.txt", NULL, hwnd, false);

	m_quadInvoker->InitializeVertexShader(device, hwnd, L"../Engine/GeoVS.hlsl", "GeoSmokeVertexShader", false);
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/GeoSmokePS.hlsl", "GeoSmokePixelShader","ps_5_0");
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/GeoSmokeGS.hlsl", "GeoSmokeGeometryShader","gs_5_0");
	m_quadInvoker->InitializeCBuffer(device, "matrixBuffer", sizeof(QuadInvoker::MatrixBufferType));
	m_quadInvoker->InitializeCBuffer(device, "LightBuffer", sizeof(QuadInvoker::LightBufferType));
	m_quadInvoker->InitializeCBuffer(device, "ParticleBuffer", sizeof(particleStruct));
	m_quadInvoker->InitializeSampler(device, "heightSampler");

	m_model->m_textures["particleTexture"] = new TextureHandler(device , L"../Engine/data/star.dds", hwnd);

	m_model->AddModelTweakVar(0.1f , "particleSize", "step=0.01 group=particle");
	m_model->AddModelTweakVar(1.0f, "incrementMod", "step=0.01 group=particle" );
	m_model->AddModelTweakVar(10.0f, "heightToKill", "step=0.01 group=particle" );

	RadIncrement = 0;
}

SmokeSceneObject::~SmokeSceneObject(void)
{
	delete m_quadInvoker;
	m_quadInvoker = 0;
	delete m_model;
	m_model = 0;
}

void SmokeSceneObject::renderTheScene(GraphicsContext & p_in)
{
	auto * invoker = m_quadInvoker;
	auto * Model = m_model;
	
	//if(RadIncrement < XM_PI/2)
	//{
	//	RadIncrement += 0.001;
	//}
	//else
	//{
	RadIncrement += p_in.deltaTime * 0.001f;
	//}

	ID3D11DeviceContext * tempDevice = p_in.GetD3DContext()->GetDeviceContext();

	XMMATRIX tempMat;
	Model->ModelMatrix(tempMat);

	QuadInvoker::MatrixBufferType *tempMatBuff = invoker->MapCBuffer<QuadInvoker::MatrixBufferType>(tempDevice , "matrixBuffer");
	tempMatBuff->worldMatrix = XMMatrixTranspose(tempMat);
	tempMatBuff->viewMatrix = XMMatrixTranspose( *p_in.c_view);
	tempMatBuff->projectionMatrix = XMMatrixTranspose(*p_in.c_projection);
	invoker->UnMapCBuffer(tempDevice , "matrixBuffer");
	tempDevice->VSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);
	tempDevice->GSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);


	particleStruct * tempParticle = invoker->MapCBuffer<particleStruct>(tempDevice, "ParticleBuffer");
	tempParticle->size = m_model->TweakVariables["particleSize"];
	tempParticle->time = RadIncrement;
	tempParticle->incrementMod = m_model->TweakVariables["incrementMod"];
	tempParticle->heightToKill = m_model->TweakVariables["heightToKill"];
	invoker->UnMapCBuffer(tempDevice, "ParticleBuffer");
	tempDevice->GSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["ParticleBuffer"]);

	ID3D11ShaderResourceView * tempArray2[1];
	tempArray2[0] = m_model->m_textures["particleTexture"]->GetTexture();
	tempDevice->PSSetShaderResources(0, 1, tempArray2);


	//p_in.c_d3d->EnableAlphaBlending();
	Model->RenderAllBuffers<ModelLightingClass::LightingVertexType> (tempDevice, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	invoker->RenderShader(tempDevice ,  Model->GetIndexCount());
	//p_in.c_d3d->DisableAlphaBlending();
}