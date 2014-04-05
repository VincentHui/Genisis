#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
#include <AntTweakBar.h>
#include "SimpleMath.h"

using namespace DirectX;

class Camera
{

	static float quat[4];
public:
	Camera(	TwBar *p_bar);
	Camera(const Camera&);
	~Camera(void);

	void setPos(float x, float y, float z);
	void setRot(float, float, float);

	XMFLOAT3 GetPos() const;
	XMFLOAT3 GetRot() const;

	void Render();
	void GetViewMatrix(XMMATRIX& _out) const;
	void RotX(float);
	void MovX(float);
	void MovY(float);
	void MovZ(float);

	void GetPosZ(float* get);
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMFLOAT3 lookAt;
	XMVECTOR q;

	XMMATRIX m_viewMatrix;
	TwBar *m_bar;
};

#endif