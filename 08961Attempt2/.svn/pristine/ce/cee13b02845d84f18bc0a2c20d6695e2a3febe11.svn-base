#pragma once 
#include <AntTweakBar.h>
#include <math.h>
#include <DirectXMath.h>
#include <vector>
const float MOVE_INCREMENT = 0.00005f;
const float MOVE_DECCREMENT = 0.00009f;
const float ROT_INCREMENT = 0.00101f;
const float ROT_DECCREMENT = 0.001f;
using namespace std;
using namespace DirectX;

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetPosition(const XMFLOAT3 &);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&)const;
	void GetRotation(float&, float&, float&) const;

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);
	void ChangePosition(bool);

private:
	struct FixedCamera{
		FixedCamera(const XMFLOAT3 & p_pos, const XMFLOAT3 & p_rot):
			Position(p_pos), Rotation(p_rot)
		{
		}
		XMFLOAT3 Position;
		XMFLOAT3 Rotation;
	};

	struct waypoint
	{
		waypoint(const XMVECTOR & p_position, const XMVECTOR & p_rotation): position(p_position), rotation(p_rotation)
		{
		}
		XMVECTOR position;
		XMVECTOR rotation;
	};

	vector <FixedCamera*> FixedCameras; 

	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	float m_frameTime;

	float m_forwardSpeed, m_backwardSpeed;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;

	float forwardSpeed;
	unsigned int CameraListElement;

	TwBar * m_Bar;
};
