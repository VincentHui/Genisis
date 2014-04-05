#include "TextureHandler.h"
#include <DDSTextureLoader.h>

TextureHandler::TextureHandler(void):m_texture(0)
{
}

TextureHandler::TextureHandler(ID3D11Device* device, WCHAR* filename,  HWND hwnd):m_texture(0)
{
	HRESULT result;
	// Load the texture in.
	//error here? ask around for advice vince
	m_texture = NULL;
	result = CreateDDSTextureFromFile(device, filename, nullptr, &m_texture);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Could not initialize texture", L"Error", MB_OK);
	}
}

TextureHandler::TextureHandler(const TextureHandler& other)
{

}

TextureHandler::~TextureHandler(void)
{
}

bool TextureHandler::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	// Load the texture in.
	
	//error here? ask around for advice vince
	m_texture = NULL;
	result = CreateDDSTextureFromFile(device, filename, nullptr, &m_texture);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureHandler::shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
	return;

}

ID3D11ShaderResourceView* TextureHandler::GetTexture()
{
	return m_texture;
}