#include <math.h>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include "PlayerMovementComponent.h"
#include "GameEngine/GameEngineMain.h"
#include "GameEngine/EntitySystem/Components/SpriteRenderComponent.h"


using namespace Game;

PlayerMovementComponent::PlayerMovementComponent()
{

}

PlayerMovementComponent::~PlayerMovementComponent()
{

}

void PlayerMovementComponent::OnAddToWorld() 
{
	Component::OnAddToWorld();
}

void PlayerMovementComponent::Update()
{
	Component::Update();

	float dt = GameEngine::GameEngineMain::GetTimeDelta();

	sf::Vector2f wantedVel = sf::Vector2f(0.f, 0.f);

	float ladders[5] = { 50, 131, 190, 250, 400};

	float jumpTimes[4] = { 0.5, 0.5, 0.5, 0.5 };
	

	float jumpHeights[4] = { 50, 50, 50, 80};

	float defaultHeight = 250;

	float playerVel = 1;

	if (jumpTime != 0) {
		playerVel = jumpDistance / jumpTime;
	}


	if (startPosition - endPosition >= 0) {
		playerVel *= -1;
	}

	if (jumpDuration == 0) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			int currX = (int)(round((GetEntity()->GetPos()).x) + 0.03);
			
			for (int i = 0; i < 4; i++) {
				if (currX == ladders[i + 1]) {
					jumpDuration = jumpTimes[i];
					maxJumpDuration = jumpDuration;
					startPosition = currX;
					endPosition = ladders[i];
					jumpHeight = jumpHeights[i];
					jumpDistance = ladders[i + 1] - ladders[i];
					jumpTime = jumpTimes[i];
				}
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			int currX = (int)(round((GetEntity()->GetPos()).x) + 0.03);

			for (int i = 0; i < 4; i++) {
				if (currX == ladders[i]) {
					jumpDuration = jumpTimes[i];
					maxJumpDuration = jumpDuration;
					startPosition = currX;
					endPosition = ladders[i+1];
					jumpHeight = jumpHeights[i];
					jumpDistance = ladders[i+1] - ladders[i];
					jumpTime = jumpTimes[i];
				}
			}
		}
	}
	else if (jumpDuration - dt <= 0) {
		wantedVel.y = -1 * GetEntity()->GetPos().y + defaultHeight;
		wantedVel.x = -1 * GetEntity()->GetPos().x + endPosition;
	}
	else {
		wantedVel.x = playerVel * dt;
		wantedVel.y = -1 * jumpHeight * (1 - pow(((GetEntity()->GetPos().x + wantedVel.x)-(startPosition+endPosition)/2),2)/pow(jumpDistance/2,2)) -GetEntity()->GetPos().y + defaultHeight;
	
	}

	if (jumpDuration > 0) {
		if (jumpDuration <= maxJumpDuration / 8 && playerVel < 0) {
			GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(1, 0);
		}
		else if (jumpDuration >= 7 * maxJumpDuration / 8 && playerVel < 0) {
			GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(1, 0);
		}
		else if (playerVel < 0) {
			GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(0, 0);
		}
		else if (jumpDuration <= maxJumpDuration / 8 && playerVel > 0) {
			GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(3, 0);
		}
		else if (jumpDuration >= 7 * maxJumpDuration / 8 && playerVel > 0) {
			GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(3, 0);
		}
		else if (playerVel > 0) {
			GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(4, 0);
		}
	}
	else {
		GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->SetTileIndex(2, 0);
	}
	



	jumpDuration -= dt;
	if (jumpDuration <= 0) jumpDuration = 0;

	

	GetEntity()->SetPos(GetEntity()->GetPos() + wantedVel);

}

