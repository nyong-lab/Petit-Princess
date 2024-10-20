#pragma once

class GameEngine;
class IGraphics;

class PetitePrincess
{
public:
	PetitePrincess() {};
	~PetitePrincess() {};

public:
	void Initialize(std::unique_ptr<GameEngine>& gameEngine, std::shared_ptr<IGraphics> graphics);
};

