#pragma once
#include <SFML/Graphics.hpp>
#include "Pipe.h"
#include "Threshold.h"

struct Gateway
{
	Pipe topPipe, bottomPipe;
	Threshold threshold;
	Gateway(Pipe& topPipe, Pipe& bottomPipe, Threshold& threshold);
	Gateway();

	void Draw(sf::RenderWindow& window);
	void MoveX(float speed, int newYOffset, float deltaTime, bool* offScreen);
	void CloseBy(float closeBy);
	void Reset();
};
