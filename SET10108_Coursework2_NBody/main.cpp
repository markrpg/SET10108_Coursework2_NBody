// SET10108_Coursework2_NBody.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <vector>
#include <random>
#include <chrono>
#include "Particle.h"
//SFML Graphics library for displaying particles
#include <SFML\Graphics.hpp>
#include <stdio.h>
#include <iostream>  
#include <fstream>
#include <math.h>

using namespace std;
using namespace std::chrono;

//Global Variables
constexpr unsigned int PARTICLECOUNT = 4096;
constexpr int ITERATIONS = 100;
constexpr int SCREENSIZE = 800;
constexpr float TIMESTEP = 10.0f;
constexpr float RESISTANCE = 0.99f;

//Function to update display of particles
void updateDisplay(sf::RenderWindow &window, vector<Particle> *particles)
{
	//Clear Window
	window.clear();
	for each (Particle var in *particles)
	{
		window.draw(var);
	}
	//Display Window
	window.display();
}

//Main Function
int main()
{
	//Create file to store times in MS
	ofstream data("NBodyBench.csv", ofstream::out);
	//Initialise Window
	sf::RenderWindow window(sf::VideoMode(SCREENSIZE, SCREENSIZE), "NBody Simulation - Mark McLaughlin - 40200606");
	//Particle Vector
	vector<Particle> particles;
	//Random Number generator
	default_random_engine e(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
	//Uniform distribution
	uniform_real_distribution<float> float_dist(0, SCREENSIZE);

	//initialise particles
	for (int i = 0; i < PARTICLECOUNT; i++)
	{
		particles.push_back(Particle(100));
		particles[i].InitParticle();
		//Set random position
		particles[i].setPosition(float_dist(e), float_dist(e));
		//Set random velocity
		particles[i].setVelocity({0.0f,0.0f});
	}

	//Keep count of iterations
	int count = 0;

	while(window.isOpen() && count < ITERATIONS)
	{
		//Start Recording time
		auto start = system_clock::now();

		//Brute-Force Pair Method - NBody
		for (int i = 0; i < PARTICLECOUNT; i++)
		{
			//Local velocity variable used to calculate new velocity
			sf::Vector2f velocity = {0.0f, 0.0f};
			for (int j = 0; j < PARTICLECOUNT; j++)
			{
				//Dont count same particle
				if (i == j)
					continue;
				//Get distance from two particles
				sf::Vector2f distance = particles[j].getPosition() - particles[i].getPosition();
				//Calculating DOT product
				float dot = (distance.x * distance.x + distance.y * distance.y) + 3e4;		 
				//If the dot product is valid
				if (dot > 0.1f)
				{
					//Get inverse distance
					float inverseDistance = pow(1.0f / sqrtf(dot), 3);
					//Add to velocity
					velocity += distance * inverseDistance;
				}
			}
			//Calculate new velocity
			sf::Vector2f newVelocity = (TIMESTEP * velocity * RESISTANCE) + particles[i].getVelocity();
			//Set Velocity
			particles[i].setVelocity(newVelocity);
			//Check for out of bounds position and fix
			sf::Vector2f position = particles[i].getPosition();
			position.x = min(max(position.x, 0.0f), (float)SCREENSIZE);
			position.y = min(max(position.y, 0.0f), (float)SCREENSIZE);
			//Update Position
			particles[i].setPosition(position + newVelocity);

		}

		//Update display
		updateDisplay(window,&particles);
		//Keep iteration count
		count++;
		//Output iteration benchmark in ms
		auto end = system_clock::now();
		auto total = end - start;
		cout << endl << "Iteration #" << count << " Benchmark MS: " << duration_cast<milliseconds>(total).count() << endl;
		data << duration_cast<milliseconds>(total).count() << endl;
	}


	//Pause 
    return 0;
}

