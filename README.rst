PyDNest
=================

This package provides

* Python connection to the C implementation of Diffusive Nested Sampling (`DNest3 <https://github.com/eggplantbren/DNest3>`_).
* Pure Python re-implementation of Diffusive Nested Sampling (diffusenested.py).

This software is in beta status -- verify the results and report bugs!

Using the Connector to DNest3
==============================

1. Compile DNest into a shared library::

	cd /dir/to/DNest3
	patch -p1 < /dir/to/PyDNest/dnest3-build-sharedlib.patch

  rebuild DNest as usual (cmake, make)
  You should find libdnest3s.so in the build directory.

2. Build the PyDNest bridge::

        cd PyDNest
        make

  You should find libdnestbridge.so in this directory.

3. You are ready to go. Try the example in run.py. First tell it where to find the two libraries you created::

	export LD_LIBRARY_PATH=/dir/to/DNest3/build/:/dir/to/PyDNest/

	python run.py

For using in your application, just import run.py and pass your functions to dnest_run. You need to define

1. **int allocate()**: makes a new particle. returns a integer (or C-pointer) to know which particle we are talking about.
2. **double drawFromPrior(i)**: gets the integer from above, and draws a position from the prior for it (storing the position somewhere). 
3. **perturb(i)**: gets the integer from above, and modifies the position. Returns H (usually 0?).
4. **likelihood(i)**: gets the integer from above, and returns the likelihood for this particle.





Author: Johannes Buchner

