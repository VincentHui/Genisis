

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h> /* remember to include the lib file! */
#include <fstream>
#include "Logger.h"
#include "LightInfo.h"

using namespace DirectX;
using namespace std;

//const XMFLOAT4 manualTess = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

class BezierInvoker
{
public:
	BezierInvoker(void);
	BezierInvoker(const BezierInvoker&);
	~BezierInvoker(void);

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	//The render function sets the shader parameters and then draws the prepared model vertices using the shader
	bool Render(ID3D11DeviceContext*, int,  const XMMATRIX& world,  const XMMATRIX& view,  const XMMATRIX& projection, float tessAmount,
		float p_tessScale, float p_tessBias, ID3D11ShaderResourceView** texture, const XMFLOAT3 & p_camPos,
		const XMFLOAT4 & p_tess_misc ,const LightInfo& LightInfo);

private:

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct BufferBase
	{
		virtual void MapPtr(const BufferBase * other)
		{

		}  
	};

	struct MatrixBufferType
	{

		void operator = (MatrixBufferType * other)
		{
			this->worldMatrix = other->worldMatrix;
			this->viewMatrix = other->viewMatrix;
			this->projectionMatrix = other->projectionMatrix;
		}

		void MapPtr(const MatrixBufferType * other)
		{
			 this->worldMatrix = other->worldMatrix;
			 this->viewMatrix = other->viewMatrix;
			 this->projectionMatrix = other->projectionMatrix;
		}  

		XMMATRIX  worldMatrix;
		 XMMATRIX  viewMatrix;
		 XMMATRIX  projectionMatrix;
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
		XMFLOAT2 padding;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};

	//bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*, WCHAR*, WCHAR*);
	bool InitializBezierShader(ID3D11Device*, HWND, WCHAR*, WCHAR*, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*,  const XMMATRIX& world,  const XMMATRIX& view,  const XMMATRIX& projection, 
		float tessAmount, float p_tessBias, float p_tessScale,
		ID3D11ShaderResourceView** texture, const XMFLOAT3 & p_camPos,
		const XMFLOAT4 & p_tess_misc ,const LightInfo& LightInfo);
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



