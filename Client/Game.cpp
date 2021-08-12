#include "pch.h"	// 자동으로 추가됨
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"



void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	GET_SINGLE(SceneManager)->LoadScene(L"TestScene");
}

void Game::Update()
{
	GEngine->Update();
}
