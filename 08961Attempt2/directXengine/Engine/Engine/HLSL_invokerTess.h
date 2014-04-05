

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h> /* remember to include the lib file! */
#include <fstream>
#include "Logger.h"
#include "LightInfo.h"

using namespace DirectX;
using namespace std;

const XMFLOAT4 manualTess = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
const XMFLOAT4 ViewLODTess = XMFLOAT4(2.0f, 0.0f, 0.0f, 0.0f);

class HLSL_invokerTess
{
public:
	HLSL_invokerTess(void);
	HLSL_invokerTess(const HLSL_invokerTess&);
	~HLSL_invokerTess(void);

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	//The render function sets the shader parameters and then draws the prepared model vertices using the shader
	bool Render(ID3D11DeviceContext*, int,  XMFLOAT4X4 ,  XMFLOAT4X4 ,  XMFLOAT4X4, float tessAmount,
		float p_tessScale, float p_tessBias, ID3D11ShaderResourceView** texture, const XMFLOAT3 & p_camPos,
		const XMFLOAT4 & p_tess_misc, const LightInfo& LightInfo);

private:

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct MatrixBufferType
	{
		 XMFLOAT4X4  worldMatrix;
		 XMFLOAT4X4  viewMatrix;
		 XMFLOAT4X4  projectionMatrix;
	};

	struct TessellationBufferType
	{
		float tessellationAmount;
		XMFLOAT3 cameraPosition;
		XMFLOAT4 misc;
	};

	struct TesDisplacementBufferType
	{
		float bias;
		float scale;
	};

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*, WCHAR*, WCHAR*);
	bool InitializBezierShader(ID3D11Device*, HWND, WCHAR*, WCHAR*, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*,  XMFLOAT4X4 ,  XMFLOAT4X4 ,  XMFLOAT4X4, 
		float tessAmount, float p_tessBias, float p_tessScale,
		ID3D11ShaderResourceView** texture, const XMFLOAT3 & p_camPos,
		const XMFLOAT4 & p_tess_misc, const LightInfo& LightInfo);
	void RenderShader(ID3D11DeviceContext*, int);


	ID3D11HullShader* m_hullShader;
	ID3D11DomainShader* m_domainShader;
	ID3D11VertexShader* m_vertexShader;
	ID3D11SamplerState* m_sampleState;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_tessellationBuffer;
	ID3D11Buffer* m_TesDisplacementBuffer;
	//
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;

};



