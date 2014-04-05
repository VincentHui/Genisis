#include "Camera.h"


Camera::Camera(TwBar *p_bar): m_positionX(0.0f), m_positionY(0.0f), m_positionZ(0.0f), m_rotationX(0.0f), m_rotationY(0.0f), m_rotationZ(0.0f),
	m_bar(p_bar)
{
	//TwAddVarRW(m_bar, "View direction", TW_TYPE_DIR3F, &lookAt, "opened=true axisz=-z showval=false group=CamVar");
	//TwAddVarRW(m_bar, "Xpos", TW_TYPE_FLOAT, &m_positionX, " step=0.1 group=CamVar  ");
	//TwAddVarRW(m_bar, "Ypos", TW_TYPE_FLOAT, &m_positionY, " step=0.1 group=CamVar ");
	//TwAddVarRW(m_bar, "ZPos", TW_TYPE_FLOAT, &m_positionZ, " step=0.1 group=CamVar  ");
	//TwAddVarRW(m_bar, "Xrot", TW_TYPE_FLOAT, &m_rotationX, " step=0.1 group=CamVar  ");
	//TwAddVarRW(m_bar, "Yrot", TW_TYPE_FLOAT, &m_rotationY, " step=0.1 group=CamVar ");
	//TwAddVarRW(m_bar, "Zrot", TW_TYPE_FLOAT, &m_rotationZ, " step=0.1 group=CamVar  ");

	q = XMQuaternionIdentity();
	
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
}

Camera::~Camera(void)
{
	//m_bar = 0;
	//delete m_bar;
}

void Camera::setPos(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void Camera::setRot(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

XMFLOAT3 Camera::GetPos() const
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 Camera::GetRot() const
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::RotX(float x)
{
	m_rotationX += x;
}

void Camera::MovX(float x)
{
	m_positionX += x;
}

void Camera::MovY(float y)
{
	m_positionY += y;
}

void Camera::MovZ(float z)
{
	m_positionZ += z;
}

//The Render function uses the position and rotation of the camera to build and update the view matrix
//also LHS rotation notation
void Camera::Render()
{
	//the goal is to apply the transformations via matrix
	//that means rotate THAN translate

	
	XMFLOAT3 up, position /*lookAt*/;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	//lookAt.x = 0.0f;
	//lookAt.y = 0.0f;
	//lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	//find calculations on how to make rotation matrices!
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw( pitch, yaw, roll);


	//converting bullshit
	XMVECTOR vec_lookAt;
	XMVECTOR vec_up;
	XMVECTOR vec_position;
	//XMVECTOR q = {0.0f, 0.0f, 0.0f, 1.0f};
	XMMatrixRotationQuaternion(q);
	vec_position = XMLoadFloat3(&position);
	//end of bullshit


	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	vec_lookAt = XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix);
	vec_up = XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	vec_lookAt = vec_position + vec_lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH (vec_position, vec_lookAt, vec_up);

}

void Camera::GetViewMatrix(XMMATRIX& p_viewMartrix) const
{
	p_viewMartrix = m_viewMatrix;
}

void Camera::GetPosZ(float* get)
{
	get = &m_positionZ;
}