#include "stdafx.h"
#include "Particle.h"
//SFML Graphics library for displaying particles
#include <SFML\Graphics.hpp>


//Function to initialise default particle settings
void Particle::InitParticle()
{
	//Setup fill color
	setFillColor(sf::Color::Cyan);
	//Set Scale
	setScale(0.01, 0.01);
	//Set velocity by default
	velocity = { 0,0 };
	Transformable t;
	velocity = t.getPosition();
}



//Set Velocity
void Particle::setVelocity(sf::Vector2f Velocity) { velocity = Velocity; }

//Get Velocity
sf::Vector2f Particle::getVelocity() { return velocity; }

