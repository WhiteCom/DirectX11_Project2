#pragma once

#include "CommonHeader_DX11.h"
#include "CommonHeader_DX11.h"

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

public:
	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_d3dContext; }
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return m_d3dDevice; }
	ID3D11ShaderResourceView* GetTexture() { return m_texture.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<DirectX::Keyboard> m_keyboard;

	DirectX::XMMATRIX m_projection;

	// Texture
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};