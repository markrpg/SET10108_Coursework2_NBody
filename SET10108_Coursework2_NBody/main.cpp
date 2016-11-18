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

using namespace std;
using namespace std::chrono;

//Global Variables
constexpr unsigned int PARTICLECOUNT = 4096;
constexpr int ITERATIONS = 10;
constexpr int SCREENSIZE = 800;

//Function to update display of particles
void updateDisplay(sf::RenderWindow &window, vector<Particle> *particles)
{
	window.clear();
	for each (Particle var in *particles)
	{
		window.draw(var);
	}

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
	uniform_int_distribution<unsigned int> int_dist(0, SCREENSIZE);
	//Local particle object for populating vector
	Particle p(100);
	p.InitParticle();

	//initialise particles
	for (int i = 0; i < PARTICLECOUNT; i++)
	{
		particles.push_back(p);
		//Set random position
		particles[i].setPosition(int_dist(e), int_dist(e));
		//Set random velocity
		particles[i].setVelocity(int_dist(e), int_dist(e));
	}

	//Keep count of iterations
	int count = 0;

	while(window.isOpen() && count < ITERATIONS)
	{
		//Start Recording time
		auto start = system_clock::now();

		for (int i = 0; i < PARTICLECOUNT; i++)
		{

			for (int i = 0; i < PARTICLECOUNT; i++)
			{

			}
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

    return system("pause");
}

