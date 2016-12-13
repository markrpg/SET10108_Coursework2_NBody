#include "stdafx.h"
#include "DrawParticle.h"
//SFML Graphics library for displaying particles
#include <SFML\Graphics.hpp>


//Function to initialise default particle settings
void DrawParticle::InitParticle()
{
	//Setup fill color
	setFillColor(sf::Color::Cyan);
	//Set Scale
	setScale(0.01, 0.01);
	//Set velocity by default
	vx = 0.0f;
	vy = 0.0f;
}