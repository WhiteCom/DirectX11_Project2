#pragma once

#include "CommonHeader_DX11.h"

#include "Graphics.h"

#define VERTICES_NUM 4

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};

struct MatrixBuffer
{
	DirectX::XMFLOAT4X4 projection; // float 4x4 (16 bytes)
};

class SimpleProgressBar
{
public:
	SimpleProgressBar();
	~SimpleProgressBar();

	void Init(Graphics* pGfx);
	void Update(float deltaTime);
	void Render(Graphics* pGfx);
	void CompileShader(Graphics* pGfx);

	void SetProgressValue(float value) { m_progressValue = value; }
	const float GetProgressValue() { return m_progressValue; }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixConstantBuffer;
	
	// UI 추가될 경우, Graphics 클래스에서 공통적으로 관리하기 (리소스 부하가 매우 커짐)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	float m_progressValue;
	Vertex m_barVertices[VERTICES_NUM];
	Vertex m_barBoundaryVertices[VERTICES_NUM];
};