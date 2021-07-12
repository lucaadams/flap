#include "Threshold.h"
#include "Consts.h"

Threshold::Threshold(sf::Vector2f size, sf::Vector2f position)
	: startPosition(position), isEnabled(true)
{
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	body.setPosition(position);
}

void Threshold::MoveX(float speed, int defaultYOffset, float deltaTime)
{
	// check if off screen
	if (body.getPosition().x < 0 - body.getSize().x / 2.0f)
	{
		// move to right side of screen
		body.move(sf::Vector2f(WINDOW_WIDTH + body.getSize().x, 0));
		// it is certain the bird is not in the threshold anymore so re-enable
		ToggleOn();
	}

	sf::Vector2f movementOffset(-speed * deltaTime, 0.0f);
	body.move(movementOffset);
}

void Threshold::Reset()
{
	body.setPosition(startPosition);
	ToggleOn();
}
