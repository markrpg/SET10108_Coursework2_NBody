#include "stdafx.h"
#include "Particle.h"
//SFML Graphics library for displaying particles
#include <SFML\Graphics.hpp>

//Local Velocity variable
float velocityx;
float velocityy;

//Function to initialise default particle settings
void Particle::InitParticle()
{
	//Setup fill color
	setFillColor(sf::Color::Cyan);
	//Set Scale
	setScale(0.01, 0.01);
	//Set velocity by default
	velocityx = 0.0f;
	velocityy = 0.0f;
}

//Set Velocity
void Particle::setVelocity(float Velocityx, float Velocityy) { velocityx = Velocityx; velocityy = Velocityy; }

//Get Velocity x
float Particle::getVelocityx() { return velocityx; }

//Get Velocity y
float Particle::getVelocityy() { return velocityy; }
