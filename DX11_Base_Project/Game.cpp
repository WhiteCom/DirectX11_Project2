#include "Game.h"

Game::Game(HWND hWnd)
{
	m_hWnd = hWnd;
	m_pGfx = std::make_unique<Graphics>(m_hWnd);

	m_pGfx->SetProjection(DirectX::XMMatrixPerspectiveFovLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

Game::~Game()
{

}

void Game::Run()
{
	DoFrame();
}

void Game::DoFrame()
{
	float deltaTime = m_Timer.Mark();

	const float color_red = 0.5f;
	const float color_blue = 0.5f;
	const float color_green = 0.5f;

	m_pGfx->ClearBuffer(color_red, color_blue, color_green);

	// To do...
	// Render Something

	m_pGfx->EndFrame();
}