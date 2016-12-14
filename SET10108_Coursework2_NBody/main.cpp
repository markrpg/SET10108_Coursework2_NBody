// SET10108_Coursework2_NBody.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <vector>
#include <random>
#include <chrono>
#include "Particle.h"
//SFML Graphics library for displaying particles
#include <SFML\Graphics.hpp>
#include <stdio.h>
#include <amp.h>
#include <amp_math.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;
using namespace std::chrono;
using namespace Concurrency;
using namespace Concurrency::fast_math;

//Global Variables
constexpr unsigned int PARTICLECOUNT = 65000;
constexpr int ITERATIONS = 100;
constexpr int SCREENSIZE = 800;
constexpr float TIMESTEP = 10.0f;
constexpr float RESISTANCE = 0.99f;

// structure holding particle data
struct point
{
	float x;
	float y;
	float vx;
	float vy;
};

//Print Graphics Accelerator info
void print_amp_info()
{
	//Create accelerator
	accelerator acc;

	wcout << "Device: " << acc.description << endl;
	wcout << "Dedicated Memory: " << acc.dedicated_memory / 1024 << " MB" << endl;
	wcout << "Supports Shared Memory: " << acc.supports_cpu_shared_memory << endl;
	system("pause");
}

//Function to update display of particles
void updateDisplay(sf::RenderWindow &window, point *particles)
{
	//Clear Window
	window.clear();
	for (size_t i = 0; i < PARTICLECOUNT; i++)
	{
		Particle p = Particle(100);
		p.InitParticle();
		sf::Vector2f position = { particles[i].x,particles[i].y };
		sf::Vector2f velocity = { particles[i].vx,particles[i].vy };
		p.setPosition(position);
		p.setVelocity(velocity);
		window.draw(p);
	}

	//Display Window
	window.display();
}

//Main Function
int main()
{
	print_amp_info();
	//Create file to store times in MS
	ofstream data("NBodyBench.csv", ofstream::out);
	//Initialise Window
	sf::RenderWindow window(sf::VideoMode(SCREENSIZE, SCREENSIZE), "NBody Simulation - Mark McLaughlin - 40200606");
	//Particle Vector
	//vector<Particle> particles;
	//Random Number generator
	default_random_engine e(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
	//Uniform distribution
	uniform_real_distribution<float> float_dist(0, SCREENSIZE);

	//Create host memory for C++ Amp
	point *particles = new point[PARTICLECOUNT];

	//initialise particles
	for (int i = 0; i < PARTICLECOUNT; i++)
	{
		//Set random position
		particles[i].x = float_dist(e);
		particles[i].y = float_dist(e);
		//Set velocity
		particles[i].vx = { 0.0f };
		particles[i].vy = { 0.0f };
	}

	//Create device buffers - stored on GPU
	array_view<point, 1> point_buffer(PARTICLECOUNT, particles);

	//Keep count of iterations
	int count = 0;

	while (window.isOpen() && count < ITERATIONS)
	{
		//Start Recording time
		auto start = std::chrono::high_resolution_clock::now();

		//Brute force implementation of NBody using C++ AMP
		auto brute_force = [=](index<1> idx) restrict(amp)
		{

			//Brute-Force Pair Method - NBod
			//Local velocity variable used to calculate new velocity
			float vx = 0.0f;
			float vy = 0.0f;
			for (int ii = 0; ii < PARTICLECOUNT; ii++)
			{
				//Get distance from two particles
				float distx = point_buffer[ii].x - point_buffer[idx].x;
				float disty = point_buffer[ii].y - point_buffer[idx].y;

				//Calculating DOT product
				float dot = (distx * distx + disty * disty) + 3e4;
				//Get inverse distance
				float distSixth = dot * dot * dot;
				float inverseDistance = 1.0f / fast_math::sqrtf(dot);

				//Calculate new velocity
				vx += distx * inverseDistance;
				vy += disty * inverseDistance;
			}

			//Update Velocity
			//Calculate new velocity
			float newVelocityx = (0.01f * vx * 0.01f) + point_buffer[idx].vx;
			float newVelocityy = (0.01f * vy * 0.01f) + point_buffer[idx].vy;

			//Set Velocity
			point_buffer[idx].vx = newVelocityx;
			point_buffer[idx].vy = newVelocityy;

			//update position
			point_buffer[idx].x += newVelocityx;
			point_buffer[idx].y += newVelocityy;

		};

		//Carry out parallel for each on the brute force algorithm
		parallel_for_each(point_buffer.extent, brute_force);
		//Synchronize the point buffer containing the particles information
		point_buffer.synchronize();

		//Update display
		//updateDisplay(window, particles);
		//Keep iteration count
		count++;
		//Output iteration benchmark in ms
		auto end = std::chrono::high_resolution_clock::now();
		auto total = roundf((std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 0.000001) * 100) / 100;
		cout << endl << "Iteration #" << (count) << " Benchmark MS: " << total << endl;
		data << total << endl;
	}


	//Pause 
	return 0;
}

