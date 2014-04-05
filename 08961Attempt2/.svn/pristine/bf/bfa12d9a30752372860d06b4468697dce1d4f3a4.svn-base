
#include "positionclass.h"


PositionClass::PositionClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
	m_frameTime = 0.0f;
	m_forwardSpeed   = 0.0f;
	m_backwardSpeed  = 0.0f;
	m_upwardSpeed    = 0.0f;
	m_downwardSpeed  = 0.0f;
	m_leftTurnSpeed  = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed    = 0.0f;
	m_lookDownSpeed  = 0.0f;
	forwardSpeed = 0.0f;
	CameraListElement = 0;

	m_Bar = TwNewBar("PositionClass");
	string define = "PositionClass iconified=true";
	TwDefine(define.c_str());
	TwDefine(" PositionClass refresh=0.1 ");
	//string test = TwGetLastError();
	TwAddVarRW(m_Bar, "PosX", TW_TYPE_FLOAT, &m_positionX, "min=-100 max=100 step=0.1 group=Position");
	TwAddVarRW(m_Bar, "Posy", TW_TYPE_FLOAT, &m_positionY, "min=-100 max=100 step=0.1 group=Position");
	TwAddVarRW(m_Bar, "Posz", TW_TYPE_FLOAT, &m_positionZ, "min=-100 max=100 step=0.1 group=Position");

	TwAddVarRW(m_Bar, "RotX", TW_TYPE_FLOAT, &m_rotationX, " step=0.1 group=Position");
	TwAddVarRW(m_Bar, "Roty", TW_TYPE_FLOAT, &m_rotationY, " step=0.1 group=Position");
	TwAddVarRW(m_Bar, "Rotz", TW_TYPE_FLOAT, &m_rotationZ, " step=0.1 group=Position");

	FixedCameras.push_back(new FixedCamera(XMFLOAT3(-4.5f, -4.5f, 0.0f), XMFLOAT3(0.0f, 100.0f, 0.0f)));
	FixedCameras.push_back(new FixedCamera(XMFLOAT3(5.0f, -7.0f, 0.5f), XMFLOAT3(-80.0f, 90.0f, 0.0f)));
	FixedCameras.push_back(new FixedCamera(XMFLOAT3(4.0f, 5.0f, 0.0f), XMFLOAT3(-12.0f, -90.0f, -180.0f)));
	FixedCameras.push_back(new FixedCamera(XMFLOAT3(-5.5f, -4.0f, -1.5f), XMFLOAT3(-80.0f, -90.0f, 0.0f)));
	FixedCameras.push_back(new FixedCamera(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));

		//m_positionX = -4.5;
		//m_positionY = -4.5;
		//m_positionZ = 0.0;

		//m_rotationX = 0.0f;
		//m_rotationY = 100;
}


PositionClass::PositionClass(const PositionClass& )
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_forwardSpeed   = 0.0f;
	m_backwardSpeed  = 0.0f;
	m_upwardSpeed    = 0.0f;
	m_downwardSpeed  = 0.0f;
	m_leftTurnSpeed  = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed    = 0.0f;
	m_lookDownSpeed  = 0.0f;
	forwardSpeed = 0.0f;
	CameraListElement = 0;

	m_Bar = 0;
}


PositionClass::~PositionClass()
{
}


void PositionClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void PositionClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


void PositionClass::GetPosition(float& x, float& y, float& z) const
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}


void PositionClass::GetRotation(float& x, float& y, float& z) const
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}


void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}


void PositionClass::MoveForward(bool keydown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_forwardSpeed += m_frameTime * MOVE_INCREMENT;

		if(m_forwardSpeed > (m_frameTime * 0.03f))
		{
			m_forwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * MOVE_DECCREMENT;

		if(m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX += sinf(radians) * m_forwardSpeed;
	m_positionZ += cosf(radians) * m_forwardSpeed;

	return;
}


void PositionClass::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_backwardSpeed += m_frameTime * MOVE_INCREMENT;

		if(m_backwardSpeed > (m_frameTime * 0.03f))
		{
			m_backwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * MOVE_DECCREMENT;
		
		if(m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX -= sinf(radians) * m_backwardSpeed;
	m_positionZ -= cosf(radians) * m_backwardSpeed;

	return;
}


void PositionClass::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_upwardSpeed += m_frameTime * MOVE_INCREMENT;

		if(m_upwardSpeed > (m_frameTime * 0.03f))
		{
			m_upwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * MOVE_DECCREMENT;

		if(m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY += m_upwardSpeed;

	return;
}


void PositionClass::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_downwardSpeed += m_frameTime * MOVE_INCREMENT;

		if(m_downwardSpeed > (m_frameTime * 0.03f))
		{
			m_downwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * MOVE_DECCREMENT;

		if(m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY -= m_downwardSpeed;

	return;
}


void PositionClass::TurnLeft(bool keydown)
{
	// Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_leftTurnSpeed += m_frameTime * ROT_INCREMENT;

		if(m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime* ROT_DECCREMENT;

		if(m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationY -= m_leftTurnSpeed;

	 //Keep the rotation in the 0 to 360 range.
	if(m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

	return;
}


void PositionClass::TurnRight(bool keydown)
{
	// Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_rightTurnSpeed += m_frameTime * ROT_INCREMENT;

		if(m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* ROT_DECCREMENT;

		if(m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationY += m_rightTurnSpeed;

	 //Keep the rotation in the 0 to 360 range.
	if(m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

	return;
}


void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookUpSpeed += m_frameTime * ROT_INCREMENT;

		if(m_lookUpSpeed > (m_frameTime * 0.15f))
		{
			m_lookUpSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime* ROT_DECCREMENT;

		if(m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX -= m_lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	//if(m_rotationX > 90.0f)
	//{
	//	m_rotationX = 90.0f;
	//}

	return;
}


void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookDownSpeed += m_frameTime * ROT_INCREMENT;

		if(m_lookDownSpeed > (m_frameTime * 0.15f))
		{
			m_lookDownSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime* ROT_DECCREMENT;

		if(m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX += m_lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	//if(m_rotationX < -90.0f)
	//{
	//	m_rotationX = -90.0f;
	//}

	return;
}

void PositionClass::ChangePosition(bool keydown)
{
	//float radians;
	if(keydown)
	{

		m_positionX = FixedCameras[CameraListElement]->Position.x;
		m_positionY = FixedCameras[CameraListElement]->Position.y;
		m_positionZ = FixedCameras[CameraListElement]->Position.z;

		m_rotationX = FixedCameras[CameraListElement]->Rotation.x;
		m_rotationY = FixedCameras[CameraListElement]->Rotation.y;
		m_rotationZ = FixedCameras[CameraListElement]->Rotation.z;
		CameraListElement++;
		if(CameraListElement > FixedCameras.size() -1)
		{
			CameraListElement = 0;
		}
	}

	return;
}

void PositionClass::SetPosition(const XMFLOAT3 & p_in)
{
	m_positionX = p_in.x;
	m_positionY = p_in.y;
	m_positionZ = p_in.z;
}