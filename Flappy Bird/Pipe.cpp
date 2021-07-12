#include "Pipe.h"
#include "Consts.h"

Pipe::Pipe(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position)
	: startPosition(position)
{
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	body.setTexture(texture);
	body.setPosition(position);
}

void Pipe::MoveX(float speed, int defaultYOffset, int newYOffset, float deltaTime, bool* offScreen)
{
	// check if off screen
	if (body.getPosition().x < 0 - body.getSize().x / 2.0f)
	{
		// centre the gateway
		body.setPosition(sf::Vector2f(body.getPosition().x, defaultYOffset));
		// move to right side of screen
		body.move(sf::Vector2f(DEFAULT_WINDOW_WIDTH + body.getSize().x, newYOffset));
		// set offscreen pointer to true (bc the pipe is offscreen)
		*offScreen = true;
	}

	sf::Vector2f movementOffset(-speed * deltaTime, 0.0f);
	body.move(movementOffset);
}

void Pipe::MoveY(float yOffset)
{
	body.move(sf::Vector2f(0.0f, yOffset));
}

void Pipe::Reset()
{
	body.setPosition(startPosition);
}
