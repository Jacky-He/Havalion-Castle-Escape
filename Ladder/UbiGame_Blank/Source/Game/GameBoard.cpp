#include "GameBoard.h"
#include "iostream"
#include "GameEngine/GameEngineMain.h"
#include "Game/Components/PlayerMovementComponent.h"
#include "Game/Components/BackgroundMovementComponent.h"
#include "Game/Components/LinkedEntityComponent.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"
#include "Game/GameControls/ObstacleShower.h"
#include "GameEngine/EntitySystem/Components/CollidableComponent.h"
#include "GameEngine/Util/CollisionManager.h"
#include <vector>

using namespace Game;

GameBoard::GameBoard()
	:m_player(nullptr)
{
	CreatePlayer();
	CreateWall();
	CreateLadders();
	CreateFog();
    CreateShower();
}


GameBoard::~GameBoard()
{
}

void GameBoard::CreateShower()
{
    m_shower = new GameEngine::ObstacleShower();
    m_shower -> EnableShower();
}

void GameBoard::CreatePlayer()
{
	sf::RenderWindow* mainWindow = GameEngine::GameEngineMain::GetInstance()->GetRenderWindow();
	unsigned int winWidth = mainWindow->getSize().x;
	unsigned int winHeight = mainWindow->getSize().y;

	m_player = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_player);

	m_player->SetPos(sf::Vector2f(winWidth/2, 4 * winHeight/5));
	m_player->SetSize(sf::Vector2f(72.f, 72.f));
    std::cout << m_player->GetPos().x << std::endl;
	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(m_player->AddComponent<GameEngine::SpriteRenderComponent>());
    
	spriteRender->SetFillColor(sf::Color::Transparent);
	spriteRender->SetZLevel(9);
	spriteRender->SetTexture(GameEngine::eTexture::Player);
	spriteRender->SetTileIndex(2, 0);
	spriteRender->image = 2;
	m_player->AddComponent<PlayerMovementComponent>();
    m_player -> AddComponent<GameEngine::CollidableComponent>();
}

void GameBoard::CreateLadders()
{
	// Specify the size of the ladder image
	unsigned int ladderWidth = 76;
	unsigned int ladderHeight = 148;

	// Get the window dimensions
	sf::RenderWindow* mainWindow = GameEngine::GameEngineMain::GetInstance()->GetRenderWindow();
	unsigned int winWidth = mainWindow->getSize().x;
	unsigned int winHeight = mainWindow->getSize().y;
	int copiesStacked = (winHeight / ladderHeight + 1) * 2;

	// Create a hidden center that the ladders can follow
	ladderHiddenCenter = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(ladderHiddenCenter);
	ladderHiddenCenter->SetPos(sf::Vector2f(winWidth / 2.0, 0));
	BackgroundMovementComponent* bgComp = ladderHiddenCenter->AddComponent<BackgroundMovementComponent>();
	bgComp->SetSingleHeight(ladderHeight);

	float laddersX[5] = { 0.14, 0.31, 0.5, 0.65, 0.84 };

	for (int i = 0; i < 5; i++) {
		ladders[i] = new GameEngine::Entity*[copiesStacked];
		for (int j = 0; j < copiesStacked; j++) {
			ladders[i][j] = new GameEngine::Entity();
			GameEngine::GameEngineMain::GetInstance()->AddEntity(ladders[i][j]);
			ladders[i][j]->SetSize(sf::Vector2f(ladderWidth, ladderHeight));
			ladders[i][j]->SetPos(sf::Vector2f(winWidth *laddersX[i], winHeight - ladderHeight / 2.0 - ladderHeight * j));
			GameEngine::SpriteRenderComponent* render = ladders[i][j]->AddComponent<GameEngine::SpriteRenderComponent>();
			render->SetFillColor(sf::Color::Transparent);
			render->SetZLevel(1);
			render->SetTexture(GameEngine::eTexture::Ladder);
			LinkedEntityComponent* linkedComp = ladders[i][j]->AddComponent<LinkedEntityComponent>();
			linkedComp->SetFollowedEntity(ladderHiddenCenter);
			sf::Vector2f dif = ladders[i][j]->GetPos() - ladderHiddenCenter->GetPos();
			linkedComp->SetFollowOffset(dif);
		}
	}
}

void GameBoard::CreateWall()
{
	// Specify the size of the wall image
	unsigned int wallWidth = 640;
	unsigned int wallHeight = 640;

	// Get the window dimensions
	sf::RenderWindow* mainWindow = GameEngine::GameEngineMain::GetInstance()->GetRenderWindow();
	unsigned int winWidth = mainWindow->getSize().x;
	unsigned int winHeight = mainWindow->getSize().y;
	int copiesHor = winWidth / wallWidth + 1;
	int copiesVer = (winHeight / wallHeight + 1) * 2;

	// Create a hidden center that the wall images can follow
	wallHiddenCenter = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(wallHiddenCenter);
	wallHiddenCenter->SetPos(sf::Vector2f(winWidth / 2.0, 0));
	BackgroundMovementComponent* bgComp = wallHiddenCenter->AddComponent<BackgroundMovementComponent>();
	bgComp->SetSingleHeight(wallHeight);

	walls = new GameEngine::Entity**[copiesHor];
	for (int i = 0; i < copiesHor; i++) {
		walls[i] = new GameEngine::Entity*[copiesVer];
		for (int j = 0; j < copiesVer; j++) {
			walls[i][j] = new GameEngine::Entity();
			GameEngine::GameEngineMain::GetInstance()->AddEntity(walls[i][j]);
			walls[i][j]->SetSize(sf::Vector2f(wallWidth, wallHeight));
			walls[i][j]->SetPos(sf::Vector2f(wallWidth * (i + 0.5), winHeight - wallHeight * (j + 0.5)));
			GameEngine::SpriteRenderComponent* render = walls[i][j]->AddComponent<GameEngine::SpriteRenderComponent>();
			render->SetFillColor(sf::Color::Transparent);
			render->SetZLevel(0);
			render->SetTexture(GameEngine::eTexture::Wall);
			LinkedEntityComponent* linkedComp = walls[i][j]->AddComponent<LinkedEntityComponent>();
			linkedComp->SetFollowedEntity(wallHiddenCenter);
			sf::Vector2f dif = walls[i][j]->GetPos() - wallHiddenCenter->GetPos();
			linkedComp->SetFollowOffset(dif);
		}
	}
}

void GameBoard::CreateFog() {
	// Get the window dimensions
	sf::RenderWindow* mainWindow = GameEngine::GameEngineMain::GetInstance()->GetRenderWindow();
	int winWidth = (int) mainWindow->getSize().x;
	int winHeight = (int) mainWindow->getSize().y;

	// Set the fog dimensions
	int fogWidth = winWidth;
	int fogHeight = winWidth * 27 / 192;
	fog = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(fog);
	fog->SetSize(sf::Vector2f(fogWidth, fogHeight));
	fog->SetPos(sf::Vector2f(winWidth / 2.0, fogHeight / 2.0));
	GameEngine::SpriteRenderComponent* render = fog->AddComponent<GameEngine::SpriteRenderComponent>();
	render->SetFillColor(sf::Color::Transparent);
	render->SetZLevel(3);
	render->SetTexture(GameEngine::eTexture::Fog);
}

void GameBoard::Update()
{
    m_shower -> Update();
    using namespace GameEngine;
    
    std::vector<CollidableComponent*>& collidables = CollisionManager::GetInstance()->GetCollidables();
    GameEngine::CollidableComponent* thiscol = m_player -> GetComponent<GameEngine::CollidableComponent>();
    for (int a = 0; a < collidables.size(); ++a)
    {
        CollidableComponent* colComponent = collidables[a];
        if (colComponent == thiscol)
            continue;

        AABBRect intersection;
        AABBRect myBox = thiscol -> GetWorldAABB();
        AABBRect colideBox = colComponent -> GetWorldAABB();
        if (myBox.intersects(colideBox, intersection))
        {
            //end game
			GameEngine::GameEngineMain::GetInstance()->isRunning = false;

			m_shower->DisableShower();

        }
    }
}
