#include "LightInfo.h"

LightInfo::LightInfo(TwBar *p_bar)
{
	m_bar = TwNewBar("light");
	TwDefine("light iconified=true");
	TwSetBottomBar(m_bar);
	TwAddVarRW(m_bar, "Light direction", TW_TYPE_DIR3F, &m_direction, "opened=true axisz=-z showval=false group=LightVar");
	m_pos = XMFLOAT3(0.0f, 2.5f, 0.0f);
	m_switcher = 1.0f;
	m_rangeMod = 6.0f;

	
	TwAddVarRW(m_bar, "Switcher", TW_TYPE_FLOAT, &m_switcher, " min=0 max=1  step=0.01");
	TwAddVarRW(m_bar, "Range", TW_TYPE_FLOAT, &m_rangeMod, " min=0 step=0.01");
	TwAddVarRW(m_bar, "XPosition", TW_TYPE_FLOAT, &m_pos.x, "  step=0.1 group=Position");
	TwAddVarRW(m_bar, "YPosition", TW_TYPE_FLOAT, &m_pos.y, "  step=0.1 group=Position");
	TwAddVarRW(m_bar, "ZPosition", TW_TYPE_FLOAT, &m_pos.z, "  step=0.1 group=Position");
}

LightInfo::LightInfo(const LightInfo& other)
{
}

LightInfo::~LightInfo(void)
{
	//m_bar = 0;
	//delete m_bar;
}

void LightInfo::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightInfo::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightInfo::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);

	return;
}

void LightInfo::SetDirection(XMVECTOR vector)
{
	XMStoreFloat3(&m_direction, vector);
	//m_direction = XMFLOAT3(x, y, z);
	return;
}

void LightInfo::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
}

void LightInfo::SetSpecularPower(float power)
{
	m_specularPower = power;
}

XMFLOAT4 LightInfo::GetDiffuseColor() const
{
	return m_diffuseColor;
}

XMFLOAT4 LightInfo::GetAmbientColor() const
{
	return m_ambientColor;
}

XMFLOAT3 LightInfo::Getdirection() const
{
	return m_direction;
}

XMFLOAT4 LightInfo::GetSpecularColor() const
{
	return m_specularColor;
}

float LightInfo::GetSpecularPower() const
{
	return m_specularPower;
}
