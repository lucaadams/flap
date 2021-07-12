#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
#include "Consts.h"

class Boundary
{
public:
	Boundary(sf::Vector2f size, sf::Vector2f position);

	sf::Vector2f GetPosition() { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }

private:
	sf::RectangleShape body;
};
