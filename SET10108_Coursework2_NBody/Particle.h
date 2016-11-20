//SFML Graphics library for displaying particles
#include <SFML\Graphics.hpp>
#pragma once
class Particle : public sf::CircleShape
{
public:

	//Local Velocity variable
	sf::Vector2f velocity;

	Particle(float arg) : CircleShape(arg) {};
	void InitParticle();
	void setVelocity(sf::Vector2f);
	sf::Vector2f Particle::getVelocity();
};