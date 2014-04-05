#include "TerrainSceneObject.h"


TerrainSceneObject::TerrainSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd)
{
	m_model = new ModelLightingClass(name, quad, pos);
	m_quadInvoker = new QuadInvoker;
	m_dataStruct = new BasePartition::clientDataStruct;
	m_name = name;

	m_model->Initialize(device, "../Engine/data/plane.txt", NULL, hwnd, false);
	m_quadInvoker->InitializeVertexShader(device, hwnd, L"../Engine/LightVS.hlsl", "LightVertexShader", false);
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/LightHS.hlsl", "LightHullShader","hs_5_0");
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/LightDS.hlsl", "LightDomainShader","ds_5_0");
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/LightPS.hlsl", "LightPixelShader","ps_5_0");
	m_quadInvoker->InitializeCBuffer(device, "matrixBuffer", sizeof(QuadInvoker::MatrixBufferType));
	m_quadInvoker->InitializeCBuffer(device, "tessBuffer", sizeof(QuadInvoker::TessellationBufferType));
	m_quadInvoker->InitializeCBuffer(device, "tessDisplacementBuffer", sizeof(TesDisplacementBufferType));
	m_quadInvoker->InitializeCBuffer(device, "LightBuffer", sizeof(LightBufferType3));
	m_quadInvoker->InitializeCBuffer(device, "LightBuffer2", sizeof(LightPosBuffer));
	m_quadInvoker->InitializeCBuffer(device, "TexOffsetBuffer", sizeof(texelOffset));
	m_quadInvoker->InitializeSampler(device, "heightSampler");

	m_model->m_textures["HeightMap"] = new TextureHandler(device , L"../Engine/data/heightmapCustom.dds", hwnd);
	m_model->m_textures["terrainVariation"] = new TextureHandler(device, L"../Engine/data/dirt.dds", hwnd);
	m_model->m_textures["Grass"] = new TextureHandler(device , L"../Engine/data/grass.dds", hwnd);
	m_model->m_textures["Mountain"] = new TextureHandler(device , L"../Engine/data/mountain.dds", hwnd);
	m_model->m_textures["Snow"] = new TextureHandler(device , L"../Engine/data/Snow2.dds", hwnd);

	m_model->AddModelTweakVar(64.0f , "TerrainTessFactor", " step=0.01 group=terrain");
	m_model->AddModelTweakVar(1.28f , "TerrainScale", " step=0.01 group=terrain");
	m_model->AddModelTweakVar(0.0f, "LOD", "step=1 min=0 max=1 group=LOD");
	m_model->AddModelTweakVar(5.0f, "LODModifier", "step=1 min=0 group=LOD");
	m_model->AddModelTweakVar(0.001f, "Texeloff", "step=0.0001 group=terrain");
	m_model->AddModelTweakVar(6.0f, "rangeModifier", "step=0.1 group=terrain");
	//m_model->AddModelTweakVar(1.0f, "switcher", "step=0.01 min=0 max=1 group=terrain");
	m_dataStruct->m_dev = device;
	m_dataStruct->m_h = hwnd;
	m_dataStruct->m_q = m_quadInvoker;

	TwAddButton(m_model->getBar() ,"FractionalEven", &TerrainSceneObject::FractionalEven, m_dataStruct, "" );
	TwAddButton(m_model->getBar() ,"FractionalOdd", &TerrainSceneObject::FractionalOdd, m_dataStruct, "" );
	TwAddButton(m_model->getBar() ,"FractionalInt", &TerrainSceneObject::Integer, m_dataStruct, "" );


}


TerrainSceneObject::~TerrainSceneObject(void)
{
	delete m_quadInvoker;
	m_quadInvoker = 0;
	delete m_model;
	m_model = 0;
	delete m_dataStruct;
	m_dataStruct = 0;
}

void TerrainSceneObject::renderTheScene( GraphicsContext & p_in)
{
	auto * invoker = m_quadInvoker;
	auto * Model = m_model;
	auto * tempLightPos = &p_in.c_lightInfo->GetPos();

	ID3D11DeviceContext * tempDevice = p_in.GetD3DContext()->GetDeviceContext();

	TesDisplacementBufferType *tempTessDispBuff = invoker->MapCBuffer<TesDisplacementBufferType>(tempDevice, "tessDisplacementBuffer");
	tempTessDispBuff->bias =  0;
	tempTessDispBuff->scale =  m_model->TweakVariables["TerrainScale"];
	tempTessDispBuff->XY = XMFLOAT2(1.0, 1.0);
	invoker->UnMapCBuffer(tempDevice, "tessDisplacementBuffer");
	tempDevice->DSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["tessDisplacementBuffer"]);

	XMMATRIX tempMat;
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
	tempTessBuff->misc = XMFLOAT3( m_model->TweakVariables["LOD"], 0, 0);
	tempTessBuff->tessellationAmount = m_model->TweakVariables["TerrainTessFactor"];
	tempTessBuff->LODModifier = m_model->TweakVariables["LODModifier"];
	invoker->UnMapCBuffer(tempDevice , "tessBuffer");
	tempDevice->HSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["tessBuffer"]);

	LightBufferType3 *tempLightBuff = invoker->MapCBuffer<LightBufferType3>(tempDevice, "LightBuffer");
	tempLightBuff->ambientColor = p_in.c_lightInfo->GetAmbientColor();
	tempLightBuff->diffuseColor = p_in.c_lightInfo->GetDiffuseColor();
	tempLightBuff->lightDirection = p_in.c_lightInfo->Getdirection();
	tempLightBuff->switcher = p_in.c_lightInfo->GetSwitcher();
	invoker->UnMapCBuffer(tempDevice, "LightBuffer");
	tempDevice->PSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["LightBuffer"]);

	LightPosBuffer *tempLightBuff2 = invoker->MapCBuffer<LightPosBuffer>(tempDevice, "LightBuffer2");
	tempLightBuff2->LightPos = p_in.c_lightInfo->GetPos();
	tempLightBuff2->rangeMod = p_in.c_lightInfo->GetRange();
	invoker->UnMapCBuffer(tempDevice, "LightBuffer2");
	tempDevice->PSSetConstantBuffers(1, 1, &invoker->GetBufferMap()["LightBuffer2"]);

	texelOffset *tempTexBuffer= invoker->MapCBuffer<texelOffset>(tempDevice, "TexOffsetBuffer");
	tempTexBuffer->texoffset =  m_model->TweakVariables["Texeloff"];
	tempTexBuffer->padding;
	invoker->UnMapCBuffer(tempDevice, "TexOffsetBuffer");
	tempDevice->PSSetConstantBuffers(2, 1, &invoker->GetBufferMap()["TexOffsetBuffer"]);

	ID3D11ShaderResourceView* tempArray2[5];
	tempArray2[0] =  m_model->m_textures["HeightMap"]->GetTexture();
	tempArray2[1] =  m_model->m_textures["Grass"]->GetTexture();
	tempArray2[2] =  m_model->m_textures["Mountain"]->GetTexture();
	tempArray2[3] = m_model->m_textures["Snow"]->GetTexture();
	tempArray2[4] = m_model->m_textures["terrainVariation"]->GetTexture();

	tempDevice->DSSetShaderResources(0, 1, &tempArray2[0]);
	tempDevice->PSSetShaderResources(0, 5, tempArray2);

	
	Model->RenderAllBuffers<ModelLightingClass::LightingVertexType> (tempDevice, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	invoker->RenderShader(tempDevice ,  Model->GetIndexCount());
}