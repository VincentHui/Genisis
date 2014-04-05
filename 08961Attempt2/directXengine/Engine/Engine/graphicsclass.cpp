////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"



GraphicsClass::GraphicsClass(int screenWidth, int screenHeight, HWND hwnd, D3DClass * p_d3d, float nearPlane, float farPlane):  
	m_LightInfo(0), Quaternion(XMQuaternionIdentity())
{
	m_D3D = 0;
	m_camera = 0;
	m_rasterState = 0;

	renderMode = 0;

	rasterDescSolid.AntialiasedLineEnable = false;
	rasterDescSolid.CullMode = D3D11_CULL_BACK;
	rasterDescSolid.DepthBias = 0;
	rasterDescSolid.DepthBiasClamp = 0.0f;
	rasterDescSolid.DepthClipEnable = true;
	rasterDescSolid.FillMode = D3D11_FILL_SOLID;
	rasterDescSolid.FrontCounterClockwise = false;
	rasterDescSolid.MultisampleEnable = false;
	rasterDescSolid.ScissorEnable = false;
	rasterDescSolid.SlopeScaledDepthBias = 0.0f;

	rasterDescWire.AntialiasedLineEnable = false;
	rasterDescWire.CullMode = D3D11_CULL_BACK;
	rasterDescWire.DepthBias = 0;
	rasterDescWire.DepthBiasClamp = 0.0f;
	rasterDescWire.DepthClipEnable = true;
	rasterDescWire.FillMode = D3D11_FILL_WIREFRAME;
	rasterDescWire.FrontCounterClockwise = false;
	rasterDescWire.MultisampleEnable = false;
	rasterDescWire.ScissorEnable = false;
	rasterDescWire.SlopeScaledDepthBias = 0.0f;

	m_D3D = p_d3d;
	m_near = nearPlane;
	m_far = farPlane;

	TwInit(TW_DIRECT3D11, m_D3D->GetDevice());
	TwWindowSize(screenWidth, screenHeight);

	m_bar = TwNewBar("Graphics");
	TwAddVarRW(m_bar, "renderMode", TW_TYPE_INT32, &renderMode, "max=1 min=0");
	TwAddVarRW(m_bar, "World Rotation", TW_TYPE_QUAT4F, &Quaternion, "opened=true axisz=-z group=Graphics");

	m_camera = new Camera(m_bar);
	m_camera->setPos(0.0f, 2.0f, -7.3f);

	m_LightInfo = new LightInfo(m_bar);
	m_LightInfo->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightInfo->SetDirection(-0.8f, -0.8f, 0.4f);
	m_LightInfo->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_LightInfo->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightInfo->SetSpecularPower(32.0f);

	m_sceneObjects["rayMarch"] = new RayMarchSceneObject("rayMarchSceneObject", false, XMFLOAT3(0, 3, 10), m_D3D->GetDevice(), hwnd, screenWidth, screenHeight);
	m_sceneObjects["torus"] = new TorrusSceneObject("TorrusSceneObject", true, XMFLOAT3(0, 0, 0), m_D3D->GetDevice(), hwnd);
	m_sceneObjects["smoke"] = new SmokeSceneObject("SmokeSceneObject", true, XMFLOAT3(0.0f, -10.0f, 10.0f), m_D3D->GetDevice(), hwnd);
	m_sceneObjects["terrain"] = new TerrainSceneObject("TerrainSceneObject", false, XMFLOAT3(0, 0, 0), m_D3D->GetDevice(), hwnd);
	m_sceneObjects["explosion"] = new GeoExplosionSceneObject ("explosionSceneObject", false, XMFLOAT3(0, -10.0f, -5.0f), m_D3D->GetDevice(), hwnd);
	m_sceneObjects["worm"] = new BezierSceneObject ("wormSceneObject", false, XMFLOAT3(0, -10, 0), m_D3D->GetDevice(), hwnd);
	m_sceneObjects["Ellipsoid"] = new EllipsoidSceneObject("Ellipsoid", false, XMFLOAT3(0, 10, 0), m_D3D->GetDevice(), hwnd);
	m_sceneObjects["Alien"] = new AlienSceneObject("Alien", true, XMFLOAT3(0, 0, 0), m_D3D->GetDevice(), hwnd);
	m_sceneObjects["GeoAlien"] = new GeoAlienSceneObject("GeoAlien", true, XMFLOAT3(0, 0, 0), m_D3D->GetDevice(), hwnd);

	m_texShader = new QuadInvoker;
	m_texShader->InitializeVertexShader(m_D3D->GetDevice(), hwnd, L"../Engine/ScreenSpaceVS.hlsl", "ScreenSpaceVertexShader", false);
	m_texShader->InitializeShaderProgram(m_D3D->GetDevice(), hwnd, L"../Engine/ColorCorrectionPS.hlsl", "ColorCorrectionPS","ps_5_0");
	m_texShader->InitializeCBuffer(m_D3D->GetDevice(), "matrixBuffer", sizeof(QuadInvoker::MatrixBufferType));
	m_texShader->InitializeCBuffer(m_D3D->GetDevice(), "HSLBuffer", sizeof(QuadInvoker::HSL));

	m_ortho = new OrthoWindowClass;
	m_ortho->Initialize( m_D3D->GetDevice(), screenWidth, screenHeight);

	m_renderTexture = new RenderTextureClass;
	m_renderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);

	AddModelTweakVar(0.20f , "hue", "step=0.01 group=HSL");
	AddModelTweakVar(0.36f , "brightness", "step=0.01 group=HSL");
	AddModelTweakVar(1.60f , "contrast", "step=0.01 group=HSL");
	AddModelTweakVar(2.04f , "saturation", "step=0.01 group=HSL");
	AddModelTweakVar(1.0f , "influence", "step=0.01 max=1 min=0 group=HSL");


	TwAddButton(m_bar ,"solid", GraphicsClass::rasterStateSolid, this, "" );
	TwAddButton(m_bar ,"wire", GraphicsClass::rasterStateWire, this, "" );

	this->rasterStateSolid(this);
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{

	for ( auto local_it = m_sceneObjects.begin(); local_it!= m_sceneObjects.end(); ++local_it )
	{
		if(local_it->second)
		{
			delete local_it->second;
			local_it->second = 0;
		}
	}

	if(m_LightInfo)
	{
		delete m_LightInfo;
		m_LightInfo = 0;
	}

	if(m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	if(m_texShader)
	{
		delete m_texShader;
	}

	if(m_renderTexture)
	{
		delete m_renderTexture;
	}

	//delete clientData;
	//clientData = 0;

	//delete clientData2;
	//clientData2 = 0;

	TwTerminate();

	return;

}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{

	rotation = 0.0f;

	return true;
}


void GraphicsClass::Shutdown()
{


}

bool GraphicsClass::Frame(int mouseX, int mouseY, XMFLOAT3 pos, XMFLOAT3 rot, float width, float height, float time)
{
	bool result;

	m_camera->setPos(pos.x, pos.y, pos.z);
	m_camera->setRot(rot.x, rot.y, rot.z);

	rotation += (float)XM_PI * 0.001f * time ;

	result = Render(rotation, width, height, time);
	if(!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::GetQuat(XMVECTOR * in)
{
	in = &Quaternion;
}

bool GraphicsClass::Render(float rotation, float screenWidth, float screenHeight, const float & deltaTime)
{
	// Clear the buffers to begin the scene.
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;

	m_camera->Render();
	m_camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	ID3D11DeviceContext * tempDevice = m_D3D->GetDeviceContext();

	//XMVECTOR rotaxis = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX quatrot = XMMatrixRotationQuaternion(Quaternion);

	XMFLOAT3 tempLightPos;
	
	tempLightPos.x = sin(rotation * 0.1f) * 6;
	tempLightPos.z = cos(rotation * 0.1f) * 6;
	tempLightPos.y = 2.5;

	m_LightInfo->SetPos(tempLightPos);
	

	GraphicsContext graphicsContext;
	graphicsContext.c_cam = m_camera;
	graphicsContext.c_d3d = m_D3D;
	graphicsContext.c_lightInfo = m_LightInfo;
	graphicsContext.c_projection = &projectionMatrix;
	graphicsContext.c_view = &viewMatrix;
	graphicsContext.c_world = &quatrot;
	graphicsContext.windowWidth = screenWidth;
	graphicsContext.windowHeight = screenHeight;
	graphicsContext.farPlane = m_far;
	graphicsContext.nearPlane = m_near;
	graphicsContext.deltaTime = deltaTime;

	// Create the rasterizer state from the description we just filled out.
	switch(renderMode)
	{
	case 1 : 
		//m_D3D->GetDevice()->CreateRasterizerState(&rasterDescWire, &m_rasterState);
		//tempDevice->RSSetState(m_rasterState);

		m_D3D->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

		for ( auto local_it = m_sceneObjects.begin(); local_it!= m_sceneObjects.end(); ++local_it )
		{
			local_it->second->renderTheScene(graphicsContext);
		};
		break;

	case 0:
		//m_D3D->GetDevice()->CreateRasterizerState(&rasterDescSolid, &m_rasterState);
		//tempDevice->RSSetState(m_rasterState);

		m_renderTexture->SetRenderTarget(tempDevice, m_D3D->m_depthStencilView);
		m_renderTexture->ClearRenderTarget(tempDevice, m_D3D->m_depthStencilView, 0.0f, 0.0f, 0.0f, 1.0f);
		m_D3D->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

		for ( auto local_it = m_sceneObjects.begin(); local_it!= m_sceneObjects.end(); ++local_it )
		{

			local_it->second->renderTheScene(graphicsContext);
		}
		m_D3D->SetBackBufferRenderTarget();

		XMMATRIX tempPorj, tempMat2;
		QuadInvoker::MatrixBufferType * tempMBuff = m_texShader->MapCBuffer<QuadInvoker::MatrixBufferType>(tempDevice, "matrixBuffer");
		tempMBuff->worldMatrix = XMMatrixTranspose(worldMatrix);
		tempMBuff->viewMatrix = XMMatrixTranspose( viewMatrix);
		tempMBuff->projectionMatrix = XMMatrixTranspose(projectionMatrix);
		m_texShader->UnMapCBuffer(tempDevice , "matrixBuffer");
		tempDevice->VSSetConstantBuffers(0, 1, &m_texShader->GetBufferMap()["matrixBuffer"]);
		ID3D11ShaderResourceView * shadArray[1]; shadArray[0] = m_renderTexture->GetShaderResourceView();
		tempDevice->PSSetShaderResources(0, 1, &shadArray[0]);

		QuadInvoker::HSL * tempHSLbuffer = m_texShader->MapCBuffer<QuadInvoker::HSL>(tempDevice, "HSLBuffer");
		tempHSLbuffer->Brightness = TweakVariables["brightness"];
		tempHSLbuffer->Contrast = TweakVariables["contrast"];
		tempHSLbuffer->Hue = TweakVariables["hue"];
		tempHSLbuffer->Saturation = TweakVariables["saturation"];
		tempHSLbuffer->influence = TweakVariables["influence"];
		tempHSLbuffer->padding = XMFLOAT3();
		tempDevice->PSSetConstantBuffers( 0,1, &m_texShader->GetBufferMap()["HSLBuffer"]);

		m_ortho->Render(m_D3D->GetDeviceContext());
		m_texShader->RenderShader(m_D3D->GetDeviceContext(),m_ortho->GetIndexCount());
		break;
	}

	return true;
}

ID3D11Device* GraphicsClass::GetD3DDevice()
{
	return m_D3D->GetDevice();
}