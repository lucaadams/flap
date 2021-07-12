#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
#include "Consts.h"

class Bird
{
public:
	Bird(sf::Texture* texture, sf::Vector2u imageCount, float jumpHeight);

	void Update(float deltaTime, bool& jumped);
	void UpdateAnimation(float deltaTime);
	void Draw(sf::RenderWindow& window) { window.draw(body); }
	void Reset();

	const sf::Vector2f GetPosition() const { return body.getPosition(); }
	const sf::Vector2f GetSize() const { return body.getSize(); }
	Collider GetCollider() { return Collider(body); }

private:
	sf::RectangleShape body;
	sf::IntRect textureRect;
	unsigned int row;

	sf::Vector2f startPosition;
	float velocityY;
	float jumpHeight;
	bool alreadyJumped;
};
