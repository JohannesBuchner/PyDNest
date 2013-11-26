import ctypes
from ctypes import cdll, CFUNCTYPE, c_int, c_double, c_void_p, create_string_buffer
lib = cdll.LoadLibrary('./libdnestbridge.so')

allocate_type = CFUNCTYPE(c_int)
perturb_type = CFUNCTYPE(c_double, c_int)
drawFromPrior_type = CFUNCTYPE(c_int, c_int)
likelihood_type = CFUNCTYPE(c_double, c_int)

def dnest_run(allocate, drawFromPrior, perturb, likelihood,
	compression = 2.7182818284590451, options_file = "OPTIONS",
	seed = 0):
	"""
Functions to pass
------------------
	
**int allocate()**: 
	makes a new particle. returns a integer (or C-pointer) 
	to know which particle we are talking about.
**double drawFromPrior(i)**: 
	gets the integer from above, and draws a position from 
	the prior for it (storing the position somewhere). 
**perturb(i)**: 
	gets the integer from above, and modifies the position. 
	Returns H (usually 0?).
**likelihood(i)**: 
	gets the integer from above, and returns the likelihood 
	for this particle.

Parameters
------------
**compression**: See DNest3 documentation
**options_file**: Options file to load
**seed**: Seed to use
"""
	
	args = (allocate_type(allocate), 
		drawFromPrior_type(drawFromPrior),
		perturb_type(perturb),
		likelihood_type(likelihood))
	lib.set_callbacks(*args)
	
	s = create_string_buffer(options_file)

	lib.ext_run(c_int(seed), c_double(compression), s)


if __name__ == '__main__':
	# test example
	import numpy
	import scipy.stats

	points = []
	ndim = 1

	def allocate():
		i = len(points)
		print 'allocate: assigning %d' % i
		points.append(None)
		return i

	def drawFromPrior(i):
		#print 'drawFromPrior: %d' % i, points[i]
		points[i] = numpy.random.uniform(0, 1, size=ndim)
		print 'drawFromPrior: drew', points[i]
		return 0

	def perturb(i):
		#print 'perturb: %d' % i, points[i]
		params = points[i]
		# alter a random direction
		which = numpy.random.randint(ndim)
		params[which] += 10**(1.5 - 6 * numpy.random.uniform(0,1)) * numpy.random.normal(0, 1);
		# bounce off walls
		params[which] = numpy.fmod(params[which] + 100, 1.);
		H = 0
		#print 'perturb: new point %d' % i, points[i]
		return H

	rv = scipy.stats.norm([0.654321]*ndim, [0.01]*ndim)
	def likelihood(i):
		params = points[i]
		l = rv.logpdf(params)
		#print 'likelihood: L =', l
		if numpy.random.randint(1000) == 999:
			print 'likelihood: %d %-5.4f %-5.2f' % (i, points[i][0], l)
		return l
	dnest_run(allocate=allocate, drawFromPrior=drawFromPrior, perturb=perturb, likelihood=likelihood)

