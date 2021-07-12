#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

class Pipe
{
public:
	Pipe(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position);
	
	void MoveX(float speed, int defaultYOffset, int newYOffset, float deltaTime, bool* offScreen);
	void MoveY(float yOffset);
	void Draw(sf::RenderWindow& window) { window.draw(body); }
	void Reset();

	sf::Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }

private:
	sf::RectangleShape body;
	sf::Vector2f startPosition;
};
