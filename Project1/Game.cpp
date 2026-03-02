#include "Game.h"

Game::Game(HWND hWnd)
{
	m_hWnd = hWnd;
	m_pGfx = std::make_unique<Graphics>(m_hWnd);

	m_pGfx->SetProjection(DirectX::XMMatrixPerspectiveFovLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	//m_pGfx->SetProjection(DirectX::XMMatrixOrthographicLH(WINDOW_CLIENT_WIDTH, WINDOW_CLIENT_HEIGHT, 0.0f, 1.0f));

	// Test Code - 2026.02.23 => Must Delete
	m_bAddValue = true;
	m_testProgressValue = 0.0f;
}

Game::~Game()
{

}

void Game::Init()
{
	// To do...
	testProgress.Init(m_pGfx.get());
}

void Game::Run()
{
	DoFrame();
}

void Game::DoFrame()
{
	float deltaTime = m_Timer.Mark();

	//
	// Render Something
	//
	const float color_red = 0.2f;
	const float color_blue = 0.2f;
	const float color_green = 0.2f;

	m_pGfx->ClearBuffer(color_red, color_blue, color_green);

	// Test Code - 2026.02.18

	if (true == m_bAddValue)
		m_testProgressValue += 0.01f;
	else
		m_testProgressValue -= 0.01f;

	if (m_testProgressValue > 1.0f)
		m_bAddValue = false;
	else if (m_testProgressValue < 0.001f)
		m_bAddValue = true;

	testProgress.SetProgressValue(m_testProgressValue);
	//testProgress.SetProgressValue(0.7f);
	
	testProgress.Update(deltaTime);
	testProgress.Render(m_pGfx.get());

	m_pGfx->EndFrame();
}