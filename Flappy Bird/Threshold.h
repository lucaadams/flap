#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

class Threshold
{
public:
	Threshold(sf::Vector2f size, sf::Vector2f position);

	void MoveX(float speed, int defaultYOffset, float deltaTime);
	void Draw(sf::RenderWindow& window) { window.draw(body); }
	void ToggleOff() { isEnabled = false; }
	void ToggleOn() { isEnabled = true; }
	bool IsEnabled() { return isEnabled; }
	void Reset();

	sf::Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }

private:
	sf::RectangleShape body;
	sf::Vector2f startPosition;
	bool isEnabled;
};
