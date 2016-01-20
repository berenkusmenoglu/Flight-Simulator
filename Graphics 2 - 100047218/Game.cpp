#include "Game.h"



Game::Game()
{
	state = PAUSED;
}

void Game::PauseGame()
{
	state = PAUSED;
}
void Game::StartGame()
{
	state = PLAYING;
}
void Game::ResumeGame()
{
	state = PLAYING;
}
void Game::RestartGame()
{
	state = PAUSED;
}
void Game::StopGame()
{
	state = FINISHED;
}


