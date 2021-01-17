#pragma once
#include "GameEngine/EntitySystem/Entity.h"
#include "GameEngine/EntitySystem/Components/TextRenderComponent.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace GameEngine
{
    class ObstacleShower;
}

namespace Game
{
	//Used for storing and controlling all game related entities, as well as providing an entry point for the "game" side of application	

	class GameBoard
	{
	public:
		GameBoard();
		virtual ~GameBoard();
		void Update();
		bool IsGameOver() { return false; }
		static sf::Music BGMusic;

	private:
        void CreateShower();
		GameEngine::Entity* m_player;
		GameEngine::Entity* m_score;
		void CreatePlayer();
    
        void CreateGod();

		GameEngine::Entity* ladderHiddenCenter;
		GameEngine::Entity** ladders[5];
		void CreateLadders();

		GameEngine::Entity* wallHiddenCenter;
		GameEngine::Entity*** walls;
		void CreateWall();
        GameEngine::ObstacleShower* m_shower;

		GameEngine::Entity** fog;
		void CreateFog();
        
        GameEngine::Entity* m_god;

		GameEngine::Entity* pauseText;
		GameEngine::Entity* pauseShade;
		sf::Clock pauseClock;
		void CreatePauseMenu();
	};
}

