#pragma once
class Game
{
public:

	enum GameState
	{
		PAUSED,
		PLAYING,
		FINISHED
	};
	Game();
	GameState state;
	
	void PauseGame();
	void StartGame();
	void ResumeGame();
	void RestartGame();
	void StopGame();

};

