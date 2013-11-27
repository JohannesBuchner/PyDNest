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

#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace std;
using namespace DNest3;

#define ifverbose if(0)

extern "C" {
	#include<stdio.h>
	// callback functions to be set externally
	void * (*ext_allocate)() = NULL;
	void (*ext_fromPrior)(void * u) = NULL;
	double (*ext_perturb)(void * u) = NULL;
	double (*ext_logLikelihood)(void * u) = NULL;
	void (*ext_print)(void * u, char * s) = NULL;
	
	void set_callbacks(
		void * (*arg_allocate)(),
		void (*arg_fromPrior)(void * u),
		double (*arg_perturb)(void * u),
		double (*arg_logLikelihood)(void * u),
		void (*arg_print)(void * u, char * s)
	) {
		ifverbose printf("setting callback functions...\n");
		ext_allocate = arg_allocate;
		ext_fromPrior = arg_fromPrior;
		ext_perturb = arg_perturb;
		ext_logLikelihood = arg_logLikelihood;
		ext_print = arg_print;
		ifverbose printf("setting callback functions done.\n");
	}
	
}

MyModel::MyModel()
{
	ifverbose cout << "MyModel::constructor. calling allocate" << endl;
	u = ext_allocate();
	ifverbose cout << "MyModel::constructor. calling allocate done" << endl;
}

void MyModel::fromPrior()
{
	v = randomU();
	ifverbose cout << "MyModel::fromPrior. calling fromPrior" << endl;
	ext_fromPrior(u);
	ifverbose cout << "MyModel::fromPrior. calling fromPrior done" << endl;
}

double MyModel::perturb()
{
	//RandomNumberGenerator::get_instance().set_seed(1);
	//v += pow(10., 1.5 - 6*randomU())*randn();
	//v = mod(v, 1.);
	//return 0.;
	ifverbose cout << "MyModel::perturb. calling perturb" << endl;
	double H = ext_perturb(u);
	ifverbose cout << "MyModel::perturb. calling perturb done: " << H << endl;
	return H;
}

double MyModel::logLikelihood() const
{
	ifverbose cout << "MyModel::logLikelihood. calling logLikelihood" << endl;
	double w = ext_logLikelihood(u);
	double l = -log(0.01 * sqrt(2*M_PI)) - 0.5 * pow((w - 0.654321) / 0.01, 2);
	ifverbose cout << "MyModel::logLikelihood. calling logLikelihood done: " << l << endl;
	return l;
}

void MyModel::print(std::ostream& out) const
{
	ifverbose cout << "MyModel::print. calling print" << endl;
	char s[500] = "1.2 3.4 5.6\0";
	if (ext_print != NULL)
		ext_print(u, s);
	out << s << " ";
	ifverbose cout << "MyModel::print. calling print done: " << endl;
}

string MyModel::description() const
{
	return string("PyDNest");
}

