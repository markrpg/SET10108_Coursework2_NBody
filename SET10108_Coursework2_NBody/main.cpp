/// main.cpp
/// SET10108 University Coursework
/// Mark McLaughlin - 40200606

#include "stdafx.h"
#include <vector>
#include <random>
#include <chrono>
#include "DrawParticle.h"
//SFML Graphics library for displaying particles
#include <SFML\Graphics.hpp>
#include <stdio.h>
#include <iostream>  
#include <fstream>
#include <math.h>
#include <omp.h>
#include <thread>

using namespace std;
using namespace std::chrono;

//Global Variables
constexpr unsigned int PARTICLECOUNT = 512;
constexpr int ITERATIONS = 100;
constexpr int SCREENSIZE = 800;
constexpr float TIMESTEP = 10.0f;
constexpr float RESISTANCE = 0.99f;

struct Particle {
	float x;
	float y;
	float vx = 0.0f;
	float vy = 0.0f;
};

//Function to update display of particles
void updateDisplay(sf::RenderWindow &window, vector<Particle> *particles)
{
	DrawParticle p = DrawParticle(100);
	p.InitParticle();
	//Clear Window
	window.clear();
	for each (Particle var in *particles)
	{
		p.setPosition(var.x, var.y);
		window.draw(p);
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
	//Get number of supported threads for OpenMP
	auto num_threads = thread::hardware_concurrency();

	//initialise particles
	for (int i = 0; i < PARTICLECOUNT; i++)
	{
		
		//particles.push_back(Particle(100));
		//particles[i].InitParticle();
		//Set random position
		particles.push_back(Particle());
		particles[i].x = float_dist(e);
		particles[i].y = float_dist(e);
	}

	//Keep count of iterations
	int count = 0;

	while (window.isOpen() && count < ITERATIONS)
	{
		//Start Recording time
		auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for num_threads(4) schedule (dynamic)
		//Brute-Force Pair Method - NBody
		for (int i = 0; i < PARTICLECOUNT; i++)
		{
			//Local velocity variable used to calculate velocity between all other particles
			float vx = 0.0f;
			float vy = 0.0f;

			for (int j = 0; j < PARTICLECOUNT; j++)
			{
				//Get distance from two particles
				float dx = particles[j].x - particles[i].x;
				float dy = particles[j].y - particles[i].y;
				//Calculating distance squared
				float dSquared = (dx * dx + dy * dy) + 3e4;
				//Get inverse distance
				float distSixth = dSquared * dSquared * dSquared;
				float inverseDist = 1.0f / sqrtf(distSixth);
				//Add to velocity
				vx += dx * inverseDist;
				vy += dy * inverseDist;
			}
			//Calculate new velocity for original particle and update the particle
			particles[i].vx += (TIMESTEP * vx * RESISTANCE);
			particles[i].vy += (TIMESTEP * vy * RESISTANCE);
			//Update position and check for out of bounds position and fix
			particles[i].x = min(max(particles[i].x + particles[i].vx, 0.0f), (float)SCREENSIZE);
			particles[i].y = min(max(particles[i].y + particles[i].vy, 0.0f), (float)SCREENSIZE);
		}

		//Update display
		//updateDisplay(window, &particles);
		//Keep iteration count
		count++;
		//Output iteration benchmark in ms
		auto end = std::chrono::high_resolution_clock::now();
		auto total = roundf((std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 0.000001) * 100) / 100;
		cout << endl << "Iteration #" << (count) << " Benchmark MS: " << total << endl;
		data << total << endl;
	}

	data << "=AVERAGE(A1:A100)";

	//Pause 
	return 0;
}

