#pragma once

#include "Graphics.h"
#include "Timer.h"

class Game
{
public:
	Game(HWND hWnd);
	~Game();

	void Init();
	void Run();

private:
	void DoFrame();

private:
	HWND m_hWnd;
	Timer m_Timer;
	std::unique_ptr<Graphics> m_pGfx;
};