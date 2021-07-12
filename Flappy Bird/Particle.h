#pragma once
#include <SFML/Graphics.hpp>

class Particle
{
public:
	Particle(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position, sf::Vector2f initialVelocity, float rotationPerSecond);
	Particle();

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);

	unsigned int GetOpacity() { return opacity; }

private:
	sf::RectangleShape body;
	sf::Vector2f velocity;
	float rotationPerSecond;
	float opacity;
};
