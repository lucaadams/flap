#include <SFML/Graphics.hpp>
#include "Gateway.h"

Gateway::Gateway(Pipe& topPipe, Pipe& bottomPipe, Threshold& threshold)
	: topPipe(topPipe), bottomPipe(bottomPipe), threshold(threshold) {}

void Gateway::Draw(sf::RenderWindow& window)
{
	topPipe.Draw(window);
	bottomPipe.Draw(window);
	//threshold.Draw(window);
}

void Gateway::MoveX(float speed, int newYOffset, float deltaTime, bool* offScreen)
{
	topPipe.MoveX(speed, 0, newYOffset, deltaTime, offScreen);
	bottomPipe.MoveX(speed, 1200, newYOffset, deltaTime, offScreen);
	threshold.MoveX(speed, 600, deltaTime);
}

void Gateway::Reset()
{
	topPipe.Reset();
	bottomPipe.Reset();
	threshold.Reset();
}

void Gateway::CloseBy(float closeBy)
{
	topPipe.MoveY(closeBy / 2.0f);
	bottomPipe.MoveY(-closeBy / 2.0f);
}
