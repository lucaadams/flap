#include "Collider.h"

Collider::Collider(sf::RectangleShape& body) : body(body) {}

bool Collider::CheckCollision(Collider& other)
{
	// AABB collision btw
	sf::Vector2f thisPosition = GetPosition();
	sf::Vector2f thisHalfSize = GetHalfSize();
	sf::Vector2f otherPosition = other.GetPosition();
	sf::Vector2f otherHalfSize = other.GetHalfSize();

	float deltaX = thisPosition.x - otherPosition.x;
	float deltaY = thisPosition.y - otherPosition.y;

	float intersectX = abs(deltaX) - (thisHalfSize.x + otherHalfSize.x);
	float intersectY = abs(deltaY) - (thisHalfSize.x + otherHalfSize.y);

	// -20.0f instead of 0.0f to make the hitboxes less annoying (more lenient)
	if (intersectX < -30.0f && intersectY < -30.0f)
		return true;

	return false;
}
