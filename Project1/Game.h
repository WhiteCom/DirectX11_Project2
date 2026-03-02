#pragma once

#include "Graphics.h"
#include "Timer.h"

// Test Code - 2026.02.23
#include "SimpleProgressBar.h"

class Game
{
public:
	Game(HWND hWnd);
	~Game();

	void Init();
	void Run();
	void Update();

private:
	void DoFrame();

private:
	HWND m_hWnd;
	Timer m_Timer;
	std::unique_ptr<Graphics> m_pGfx;

	// Test Code - 2026.02.23 => Must Delete
	bool m_bAddValue;
	float m_testProgressValue;
	SimpleProgressBar testProgress;
};