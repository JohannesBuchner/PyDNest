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
	
	void set_callbacks(
		void * (*arg_allocate)(),
		void (*arg_fromPrior)(void * u),
		double (*arg_perturb)(void * u),
		double (*arg_logLikelihood)(void * u)
	) {
		ifverbose printf("setting callback functions...\n");
		ext_allocate = arg_allocate;
		ext_fromPrior = arg_fromPrior;
		ext_perturb = arg_perturb;
		ext_logLikelihood = arg_logLikelihood;
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
	ifverbose cout << "MyModel::fromPrior. calling fromPrior" << endl;
	ext_fromPrior(u);
	ifverbose cout << "MyModel::fromPrior. calling fromPrior done" << endl;
}

double MyModel::perturb()
{
	ifverbose cout << "MyModel::perturb. calling perturb" << endl;
	double H = ext_perturb(u);
	ifverbose cout << "MyModel::perturb. calling perturb done: " << H << endl;
	return H;
}

double MyModel::logLikelihood() const
{
	ifverbose cout << "MyModel::logLikelihood. calling logLikelihood" << endl;
	double l = ext_logLikelihood(u);
	ifverbose cout << "MyModel::logLikelihood. calling logLikelihood done: " << l << endl;
	return l;
}

void MyModel::print(std::ostream& out) const
{

}

string MyModel::description() const
{
	return string("");
}

