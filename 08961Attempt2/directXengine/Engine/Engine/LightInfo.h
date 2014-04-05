#pragma once
#include <DirectXMath.h>
#include <AntTweakBar.h>

using namespace DirectX;

class LightInfo
{
public:
	LightInfo(TwBar *bar);
	LightInfo(const LightInfo& other);
	~LightInfo(void);

	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);
	void SetDirection(XMVECTOR vector);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetSpecularPower(float);

	XMFLOAT4 GetDiffuseColor() const;
	XMFLOAT4 GetAmbientColor() const;
	XMFLOAT3 Getdirection() const;
	XMFLOAT4 GetSpecularColor() const;
	XMFLOAT3 GetPos() const
	{
		return m_pos;
	}
	void SetPos(const XMFLOAT3 & newPos) 
	{
		m_pos = newPos;
	}
	float GetSpecularPower() const;
	float GetSwitcher() const
	{
		return m_switcher;
	}
	float GetRange() const
	{
		return m_rangeMod;
	}
private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;

	XMFLOAT4 m_specularColor;
	TwBar *m_bar;
	XMFLOAT3 m_pos;
	float m_specularPower;
	float m_switcher;
	float m_rangeMod;
	
};

