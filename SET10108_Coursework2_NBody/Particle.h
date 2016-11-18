//SFML Graphics library for displaying particles
#include <SFML\Graphics.hpp>
#pragma once
class Particle : public sf::CircleShape
{
public:
	Particle(float arg) : CircleShape(arg) {};
	void InitParticle();
	void setVelocity(float Velocityx, float Velocityy);
	float Particle::getVelocityx();
	float Particle::getVelocityy();
};

