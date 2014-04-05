#pragma once

#include <d3d11.h>
#include <DirectXMath.h>



using namespace DirectX;

class TextureHandler
{
public:
	TextureHandler(void);
	TextureHandler(ID3D11Device* device, WCHAR* filename, HWND hwnd);
	TextureHandler(const TextureHandler&);
	~TextureHandler(void);
	//load texture
	bool Initialize(ID3D11Device*, WCHAR* filenameOfTexture);
	//unload texture
	void shutdown();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

