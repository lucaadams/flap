#include <SFML/Graphics.hpp>
#include "Bird.h"
#include "Consts.h"

Bird::Bird(sf::Texture* texture, sf::Vector2u imageCount, float jumpHeight)
	: jumpHeight(jumpHeight), startPosition(sf::Vector2f(DEFAULT_WINDOW_WIDTH / 4, DEFAULT_WINDOW_HEIGHT / 2)), row(0), velocityY(0.0f), alreadyJumped(false)
{
	body.setSize(sf::Vector2f(150.0f, 106.0f));
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(sf::Vector2f(DEFAULT_WINDOW_WIDTH / 4, DEFAULT_WINDOW_HEIGHT / 2));
	
	body.setTexture(texture);
	textureRect.width = texture->getSize().x / float(imageCount.x);
	textureRect.height = texture->getSize().y / float(imageCount.y);
}

void Bird::Update(float deltaTime, bool& jumped)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (!alreadyJumped)
		{
			velocityY = -sqrtf(2.0f * GRAVITY * jumpHeight);
			jumped = true; // for external usage
		}
		alreadyJumped = true; // private var
	}
	else
		alreadyJumped = false;

	velocityY += GRAVITY * deltaTime;

	// manage rotation over various velocities
	if (velocityY < -800)
		body.setRotation(-20.0f);
	else if (velocityY > 1400)
		body.setRotation(35.0f);
	//else if (velocityY < -500)
	//	body.setRotation(velocityY / 60.0f);
	else
		body.setRotation(velocityY / 40.0f);

	// switch animation rect row depending if the bird is going up or down
	if (velocityY > 0.0f)
		row = 0;
	else
		row = 1;

	UpdateAnimation(deltaTime);
	body.move(sf::Vector2f(0.0f, velocityY) * deltaTime);
}

void Bird::UpdateAnimation(float deltaTime)
{
	textureRect.top = row * textureRect.height;
	body.setTextureRect(textureRect);
}

void Bird::Reset()
{
	body.setPosition(startPosition);
	velocityY = 0.0f;
}
