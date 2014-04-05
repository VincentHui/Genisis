#include "RayMarchSceneObject.h"


RayMarchSceneObject::RayMarchSceneObject(string name, bool quad, const XMFLOAT3 & pos, ID3D11Device* device,  HWND hwnd, int screenWidth, int screenHeight)
{
	m_model = new ModelLightingClass(name, quad, pos);
	updateIncrement = 0;
	q = XMQuaternionIdentity();
	BackColor =XMFLOAT4(0.4f, 0.2f, 0.0f, 1.0f);
	SphereColorsToG[0] = XMFLOAT4(1.0f, 0.4f, 0.0f, 1.0f);
	SphereColorsToG[1] = XMFLOAT4(1.0f, 0.4f, 0.0f, 1.0f);
	SphereColorsToG[2] = XMFLOAT4(1.0f, 0.4f, 0.4f, 1.0f);


	m_model->Initialize(device, "../Engine/data/plane.txt", NULL, hwnd, false);
	m_model->m_textures["Grass"] = new TextureHandler(device , L"../Engine/data/testGrass.dds", hwnd);
	m_model->AddModelTweakVar(186.0f , "shininess", " step=0.1 group=raymarch");
	m_model->AddModelTweakVar(1.4f , "levelVal", " step=0.01 group=raymarch");
	m_model->AddModelTweakVar(1.0f , "display", " step=1 min=0 max=1 group=raymarch");
	TwAddVarRW(m_model->m_bar, "RayRotation", TW_TYPE_QUAT4F, &q, "axisz=-z group=raymarchCam");
	//colours
	TwAddVarRW(m_model->m_bar, "BackColor", TW_TYPE_COLOR4F, &BackColor, "group=rayMarchColor");
	TwAddVarRW(m_model->m_bar, "SphereColor1", TW_TYPE_COLOR4F, &SphereColorsToG[0], "group=rayMarchColor");
	TwAddVarRW(m_model->m_bar, "SphereColor2", TW_TYPE_COLOR4F, &SphereColorsToG[1], "group=rayMarchColor");
	TwAddVarRW(m_model->m_bar, "SphereColor3", TW_TYPE_COLOR4F, &SphereColorsToG[2], "group=rayMarchColor");

	//cam positions
	m_model->AddModelTweakVar(-6.0f , "posX", " step=0.01  group=raymarchCam");
	m_model->AddModelTweakVar(-5.0f , "posY", " step=0.01  group=raymarchCam");
	m_model->AddModelTweakVar(-64.16f , "posZ", " step=0.01  group=raymarchCam");


	m_model->SetRot(XMFLOAT3(-1.57f, 0.0f, 0.0f));
	m_model->SetScale(XMFLOAT3(0.25f, 0.25f, 0.25f));

	m_quadInvoker = new QuadInvoker();
	m_quadInvoker->InitializeVertexShader(device, hwnd, L"../Engine/SimpleTextureVS.hlsl", "SimpleVertexShader", false);
	m_quadInvoker->InitializeShaderProgram(device, hwnd, L"../Engine/SimpleTexturePS.hlsl", "SimplePixelShader","ps_5_0");
	m_quadInvoker->InitializeCBuffer(device, "matrixBuffer", sizeof(QuadInvoker::MatrixBufferType));
	m_quadInvoker->InitializeCBuffer(device, "RayLightBuffer", sizeof(RayLightBufferType));
	m_quadInvoker->InitializeCBuffer(device, "viewportBuffer", sizeof(ViewPortInfo));
	m_quadInvoker->InitializeCBuffer(device, "viewInverse", sizeof(ViewInverseStruct));
	m_quadInvoker->InitializeCBuffer(device, "modifiers", sizeof(modifiers));
	m_quadInvoker->InitializeCBuffer(device, "sphereColors", sizeof(SphereColors));
	m_quadInvoker->InitializeSampler(device, "heightSampler");

	RayCamLookat.x = 0.0f;
	RayCamLookat.y = 0.0f;
	RayCamLookat.z = 1.0f;

	RayCamPos.x = 0.8f;
	RayCamPos.y = -0.2f;
	RayCamPos.z = -49.4f;

	RayCamRot.x = 0.0f;
	RayCamRot.y = 0.0f;
	RayCamRot.z = 0.0f;
}


RayMarchSceneObject::~RayMarchSceneObject(void)
{
	delete m_quadInvoker;
	m_quadInvoker = 0;
	delete m_model;
	m_model = 0;
}

void RayMarchSceneObject::deleteDerived()
{
	delete this;
}

void RayMarchSceneObject::renderTheScene( GraphicsContext & p_graphicsContext)
{
	updateIncrement += p_graphicsContext.deltaTime * 0.01f;
	auto * invoker = m_quadInvoker;
	auto * Model = m_model;
	ID3D11DeviceContext * tempDevice = p_graphicsContext.GetD3DContext()->GetDeviceContext();
	XMMATRIX world, view, projection;
	p_graphicsContext.c_d3d->GetWorldMatrix(world);
	view = *p_graphicsContext.c_view;
	p_graphicsContext.c_d3d->GetProjectionMatrix(projection);

	renderRayCam();

	//m_renderTexture->SetRenderTarget(tempDevice, p_graphicsContext.c_d3d->m_depthStencilView);
	//m_renderTexture->ClearRenderTarget(tempDevice, p_graphicsContext.c_d3d->m_depthStencilView, 0.0f, 0.0f, 1.0f, 1.0f);


	RayLightBufferType * tempRayLightBuffer = invoker->MapCBuffer<RayLightBufferType>(tempDevice, "RayLightBuffer");
	tempRayLightBuffer->ambientColour = p_graphicsContext.c_lightInfo->GetAmbientColor();
	tempRayLightBuffer->backgroundColour = BackColor;
	tempRayLightBuffer->lightColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	tempRayLightBuffer->lightIntensity = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	tempRayLightBuffer->lightPosition = XMFLOAT3(-0.6f, 0.0f, -61.47f);
	tempRayLightBuffer->padding = 0.0f;
	invoker->UnMapCBuffer(tempDevice, "RayLightBuffer");
	tempDevice->PSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["RayLightBuffer"]);

	ViewPortInfo * tempViewPortBuffer = invoker->MapCBuffer<ViewPortInfo>(tempDevice, "viewportBuffer");
	tempViewPortBuffer->eyePos.x = m_model->TweakVariables["posX"];
	tempViewPortBuffer->eyePos.y = m_model->TweakVariables["posY"];
	tempViewPortBuffer->eyePos.z = m_model->TweakVariables["posZ"];
	tempViewPortBuffer->eyePos.w = 1.0f;
	tempViewPortBuffer->farPlane = p_graphicsContext.farPlane;
	tempViewPortBuffer->nearPlane = 5;
	tempViewPortBuffer->viewportW = p_graphicsContext.windowWidth;
	tempViewPortBuffer->viewportH = p_graphicsContext.windowHeight;
	invoker->UnMapCBuffer(tempDevice, "viewportBuffer");
	tempDevice->PSSetConstantBuffers(1, 1,  &invoker->GetBufferMap()["viewportBuffer"]);

	ViewInverseStruct * tempViewInverseBuffer = invoker->MapCBuffer<ViewInverseStruct>(tempDevice, "viewInverse");
	tempViewInverseBuffer->InverseView = RayInverseViewMatrix;
	invoker->UnMapCBuffer(tempDevice, "viewInverse");
	tempDevice->PSSetConstantBuffers(2, 1, &invoker->GetBufferMap()["viewInverse"]);


	if(m_model->TweakVariables["display"] > 0.5f)
	{
		XMMATRIX tempPorj, tempMat2;
		XMFLOAT3 tempPos = Model->GetPos();
		tempPos.x = -5.0f;
		Model->SetPos(tempPos);
		Model->ModelMatrix(tempMat2);

		QuadInvoker::MatrixBufferType * tempMBuff = invoker->MapCBuffer<QuadInvoker::MatrixBufferType>(tempDevice, "matrixBuffer");
		tempMBuff->worldMatrix = XMMatrixTranspose(tempMat2);
		tempMBuff->viewMatrix = XMMatrixTranspose( view);
		tempMBuff->projectionMatrix = XMMatrixTranspose(*p_graphicsContext.GetProjectionContext());
		invoker->UnMapCBuffer(tempDevice , "matrixBuffer");
		tempDevice->VSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);


		modifiers * tempModBuffer = invoker->MapCBuffer<modifiers>(tempDevice, "modifiers");
		tempModBuffer->levelVal = m_model->TweakVariables["levelVal"] + 1.0f + (sin(updateIncrement * 0.1f) * 0.1f);
		tempModBuffer->shininess = m_model->TweakVariables["shininess"];
		tempModBuffer->padding = XMFLOAT2();
		invoker->UnMapCBuffer(tempDevice, "modifiers");
		tempDevice->PSSetConstantBuffers(3, 1, &invoker->GetBufferMap()["modifiers"]); 
		SphereColors * tempSphereColBuff = invoker->MapCBuffer<SphereColors>(tempDevice, "sphereColors");
		tempSphereColBuff->sphereColour = SphereColorsToG[0];
		tempSphereColBuff->sphereColour2 =  SphereColorsToG[1];
		tempSphereColBuff->sphereColour3 =  SphereColorsToG[2];
		invoker->UnMapCBuffer(tempDevice, "sphereColors");
		tempDevice->PSSetConstantBuffers(4, 1, &invoker->GetBufferMap()["sphereColors"]);

		m_model->RenderAllBuffers<ModelLightingClass::LightingVertexType>(tempDevice, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		invoker->RenderShader(p_graphicsContext.c_d3d->GetDeviceContext(),m_model->GetIndexCount());

		
		tempPos = Model->GetPos();
		tempPos.x = 5.0f;
		Model->SetPos(tempPos);
		Model->ModelMatrix(tempMat2);

		tempMBuff = invoker->MapCBuffer<QuadInvoker::MatrixBufferType>(tempDevice, "matrixBuffer");
		tempMBuff->worldMatrix = XMMatrixTranspose(tempMat2);
		tempMBuff->viewMatrix = XMMatrixTranspose( view);
		tempMBuff->projectionMatrix = XMMatrixTranspose(*p_graphicsContext.GetProjectionContext());
		invoker->UnMapCBuffer(tempDevice , "matrixBuffer");
		tempDevice->VSSetConstantBuffers(0, 1, &invoker->GetBufferMap()["matrixBuffer"]);


		tempModBuffer = invoker->MapCBuffer<modifiers>(tempDevice, "modifiers");
		tempModBuffer->levelVal = m_model->TweakVariables["levelVal"] - (sin(updateIncrement * 0.1f) * 0.3f);
		tempModBuffer->shininess = m_model->TweakVariables["shininess"];
		tempModBuffer->padding = XMFLOAT2();
		invoker->UnMapCBuffer(tempDevice, "modifiers");
		tempDevice->PSSetConstantBuffers(3, 1, &invoker->GetBufferMap()["modifiers"]); 
		tempSphereColBuff = invoker->MapCBuffer<SphereColors>(tempDevice, "sphereColors");
		tempSphereColBuff->sphereColour = SphereColorsToG[0];
		tempSphereColBuff->sphereColour2 =  SphereColorsToG[1];
		tempSphereColBuff->sphereColour3 =  SphereColorsToG[2];
		invoker->UnMapCBuffer(tempDevice, "sphereColors");
		tempDevice->PSSetConstantBuffers(4, 1, &invoker->GetBufferMap()["sphereColors"]);

		m_model->RenderAllBuffers<ModelLightingClass::LightingVertexType>(tempDevice, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		invoker->RenderShader(p_graphicsContext.c_d3d->GetDeviceContext(),m_model->GetIndexCount());
	}
}

void RayMarchSceneObject::renderRayCam()
{
	XMFLOAT3 up, position;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = RayCamPos.x;
	position.y = RayCamPos.y;
	position.z = RayCamPos.z;

	pitch = RayCamRot.x * 0.0174532925f;
	yaw   = RayCamRot.y * 0.0174532925f;
	roll  = RayCamRot.z * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw( pitch, yaw, roll);

	//converting bullshit
	XMVECTOR vec_lookAt;
	XMVECTOR vec_up;
	XMVECTOR vec_position;

	//XMMatrixRotationQuaternion(q);
	vec_position = XMLoadFloat3(&position);

	vec_lookAt = XMVector3TransformCoord(XMLoadFloat3(&RayCamLookat), rotationMatrix);
	vec_up = XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	vec_lookAt = vec_position + vec_lookAt;

	// Finally create the view matrix from the three updated vectors.

	RayInverseViewMatrix = XMMatrixLookAtLH (vec_position, vec_lookAt, vec_up) * (XMMatrixRotationQuaternion(q) * 0.1f);

}