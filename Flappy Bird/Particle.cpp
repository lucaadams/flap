#include "Particle.h"
#include "Consts.h"

Particle::Particle(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position, sf::Vector2f initialVelocity, float rotationPerSecond)
	: velocity(initialVelocity), opacity(255), rotationPerSecond(rotationPerSecond)
{
	body.setSize(size);
	body.setOrigin(size.x / 2, size.y / 2);
	body.setPosition(position);
	body.setTexture(texture);
}

Particle::Particle() : opacity(0), rotationPerSecond(0.0f) {}

void Particle::Update(float deltaTime)
{
	velocity.y -= 0.005f * velocity.y;
	velocity.x -= 0.005f * velocity.x;
	rotationPerSecond -= 0.005f * rotationPerSecond;

	body.rotate(rotationPerSecond * deltaTime);
	body.move(velocity);
	opacity -= 255.0f * 2.0f * deltaTime;
	if (opacity < 0.0f)
		opacity = 0.0f;
	body.setFillColor(sf::Color(255, 255, 255, opacity));
}

void Particle::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
