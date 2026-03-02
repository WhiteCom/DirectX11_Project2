#include "CommonHeader_Cpp.h"

#include "SimpleProgressBar.h"

SimpleProgressBar::SimpleProgressBar()
{
	m_progressValue = 0.0f;

	for (int i = 0; i < 4; ++i)
	{
		m_barVertices[i].pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_barVertices[i].color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		m_barBoundaryVertices[i].pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_barBoundaryVertices[i].color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

SimpleProgressBar::~SimpleProgressBar()
{

}

void SimpleProgressBar::Init(Graphics* pGfx)
{
	ID3D11Device* d3dDevice = pGfx->GetDevice().Get();

	if (d3dDevice == nullptr)
	{
		MessageBox(NULL, TEXT("Error : No d3dDevice! (SimpleProgressBar::Init)"), TEXT("Error"), MB_ICONERROR);
		return;
	}

	CompileShader(pGfx);

	//
	// Create D3D Buffer : Base Vertex
	//
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(m_barVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = m_barVertices;

	d3dDevice->CreateBuffer(&bd, &initData, &m_vertexBuffer);

	//
	// Create D3D Buffer : Projection Matrix Buffer
	// 
	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(MatrixBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	d3dDevice->CreateBuffer(&cbd, nullptr, &m_matrixConstantBuffer);
}

void SimpleProgressBar::Update(float deltaTime)
{
	//
	// Progress Vertex Buffer
	//
	m_progressValue = std::clamp(m_progressValue, 0.0f, 1.0f);

	float left = -0.5f;
	float right = left + m_progressValue; // progress 에 따라 width 변화
	float top = 0.1f;
	float bottom = 0.0f;

	// Test Code - 2026.02.22
	float test_z = 4.0f;

	m_barVertices[0].pos = DirectX::XMFLOAT3(left, top, test_z);
	m_barVertices[0].color = DirectX::XMFLOAT4(0, 1, 0, 1);

	m_barVertices[1].pos = DirectX::XMFLOAT3(right, top, test_z);
	m_barVertices[1].color = DirectX::XMFLOAT4(0, 1, 0, 1);

	m_barVertices[2].pos = DirectX::XMFLOAT3(left, bottom, test_z);
	m_barVertices[2].color = DirectX::XMFLOAT4(0, 1, 0, 1);

	m_barVertices[3].pos = DirectX::XMFLOAT3(right, bottom, test_z);
	m_barVertices[3].color = DirectX::XMFLOAT4(0, 1, 0, 1);
}

void SimpleProgressBar::Render(Graphics* pGfx)
{
	ID3D11DeviceContext* pD3DContext = pGfx->GetDeviceContext().Get();

	// ======================================================

	//
	// For VertexBuffer Created by DEFAULT
	// bd.Usage = D3D11_USAGE_DEFAULT;
	//
	pD3DContext->UpdateSubresource(
		m_vertexBuffer.Get(),
		0,
		nullptr,
		m_barVertices,
		0,
		0);

	//
	// For VertexBuffer Created by DYNAMIC
	// ( Must need below buffer options )
	// bd.Usage = D3D11_USAGE_DYNAMIC;
	// bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//
	HRESULT hr;

	//D3D11_MAPPED_SUBRESOURCE mapped_buffer;
	//
	//hr = pD3DContext->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);
	//if (FAILED(hr))
	//{
	//	OutputDebugStringA("Map Failed! - SimpleProgressBar::Render");
	//}
	//memcpy(mapped_buffer.pData, m_barVertices, sizeof(m_barVertices));
	//pD3DContext->Unmap(m_vertexBuffer.Get(), 0);

	// ======================================================

	MatrixBuffer cbData;
	
	DirectX::XMMATRIX proj = pGfx->GetProjection();
	proj = DirectX::XMMatrixTranspose(proj); // important

	DirectX::XMStoreFloat4x4(&cbData.projection, proj);

	D3D11_MAPPED_SUBRESOURCE mapped_matrix;

	hr = pD3DContext->Map(m_matrixConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_matrix);
	if (FAILED(hr))
	{
		OutputDebugStringA("Map Failed! - SimpleProgressBar::Render");
	}
	memcpy(mapped_matrix.pData, &cbData, sizeof(cbData));
	pD3DContext->Unmap(m_matrixConstantBuffer.Get(), 0);

	pD3DContext->VSSetConstantBuffers(1, 1, m_matrixConstantBuffer.GetAddressOf());

	// ======================================================

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pD3DContext->IASetInputLayout(m_inputLayout.Get());
	pD3DContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Must Binding Shader
	pD3DContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	pD3DContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	pD3DContext->Draw(VERTICES_NUM, 0);
}

void SimpleProgressBar::CompileShader(Graphics* pGfx)
{
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	//
	// Vetex Shader Compile
	//
	HRESULT hr;
	hr = D3DCompileFromFile(
		L"VS_ProgressBar_Simple.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		0,
		0,
		&vsBlob,
		&errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
		}
		throw::std::runtime_error("[ SimpleProgressBar - VS Complie Failed ]");
	}

	// Create Vertex Shader
	ID3D11Device* d3dDevice = pGfx->GetDevice().Get();
	d3dDevice->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		&m_vertexShader);

	//
	// Create InputLayout
	// 

	const int count_layout = 2;
	D3D11_INPUT_ELEMENT_DESC layout[count_layout] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	d3dDevice->CreateInputLayout(
		layout,
		count_layout,
		vsBlob->GetBufferPointer(), // Mist Need VS blob
		vsBlob->GetBufferSize(),
		&m_inputLayout);

	//
	// Pixel Shader Complie
	//

	hr = D3DCompileFromFile(
		L"PS_ProgressBar_Simple.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		0,
		0,
		&psBlob,
		&errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
		}
		throw::std::runtime_error("[ SimpleProgressBar - PS Complie Failed ]");
	}

	// Create Pixel Shader
	d3dDevice->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr,
		&m_pixelShader);
}