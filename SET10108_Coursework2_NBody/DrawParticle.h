//SFML Graphics library for displaying particles
#include <SFML\Graphics.hpp>
#pragma once
class DrawParticle : public sf::CircleShape
{
public:
	float x, y, vx, vy;

	DrawParticle(float arg) : CircleShape(arg) {};
	void InitParticle();
};