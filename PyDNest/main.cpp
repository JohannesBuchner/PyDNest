/*
* Copyright (c) 2009, 2010, 2011, 2012 Brendon J. Brewer.
*
* This file is part of DNest3.
*
* DNest3 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DNest3 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DNest3. If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <cstdlib>
#include "Start.h"
#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Sampler.h"
#include "Options.h"

#ifndef DNest3_No_Boost
#include "MTSampler.h"
#endif

#define ifverbose if(0)

using namespace std;
using namespace DNest3;

int run(int seed, double compression, char * optionsfile)
{
	cout << "main called" << endl;
	cout << "seed:" << seed << endl;
	cout << "compression:" << compression << endl;
	cout << "optionsfile:" << optionsfile << endl;

	RandomNumberGenerator::initialise_instance();
	RandomNumberGenerator::get_instance().set_seed(seed);

	// Load sampler options from file
	Options samplerOptions(optionsfile);

	std::cout<<"# Using "<<1<<" thread"<<
		((1 == 1)?("."):("s."))<<std::endl;

	std::cout<<"# Target compression factor between levels = ";
	std::cout<< compression <<std::endl;

	// Seed random number generator
	std::cout<<"# Seeding random number generator with "<<
		seed <<"."<<std::endl;

	// Create sampler
	#ifndef DNest3_No_Boost
	MTSampler<MyModel> sampler(1, compression, samplerOptions);
	#else
	Sampler<MyModel> sampler(samplerOptions);
	#endif
	cout << "running sampler:" << endl;
	sampler.run();
	cout << "sampler done." << endl;
	return 0;
}

extern "C" {
	#include<stdio.h>
	int ext_run(int seed, double compression, char * optionsfile) {
		return run(seed, compression, optionsfile);
	}
}

