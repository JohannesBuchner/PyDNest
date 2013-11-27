import ctypes
from ctypes import cdll, CFUNCTYPE, c_int, c_double, c_char_p, c_char, c_void_p, create_string_buffer
lib = cdll.LoadLibrary('./libdnestbridge.so')

allocate_type = CFUNCTYPE(c_int)
perturb_type = CFUNCTYPE(c_double, c_int)
drawFromPrior_type = CFUNCTYPE(c_int, c_int)
likelihood_type = CFUNCTYPE(c_double, c_int)
print_type = CFUNCTYPE(c_void_p, c_int, ctypes.POINTER(ctypes.c_char))

def dnest_run(allocate, drawFromPrior, perturb, likelihood, user_print=None,
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

	def ext_print(i, s):
		#_buffer = create_string_buffer('%f ' % points[i])
		c = ctypes.cast(s, ctypes.POINTER(ctypes.c_char))
		addr = ctypes.addressof(s.contents)
		c2 = (c_char*500).from_address(addr)
		v = user_print(i)
		#print 'print returns: "%s"' % v
		c2[:len(v)] = v
		c2[len(v)] = '\0'
		return 0
	
	args = (allocate_type(allocate), 
		drawFromPrior_type(drawFromPrior),
		perturb_type(perturb),
		likelihood_type(likelihood),
		print_type(ext_print) if user_print is not None else None)
	lib.set_callbacks(*args)
	
	s = create_string_buffer(options_file)

	lib.ext_run(c_int(seed), c_double(compression), s)


if __name__ == '__main__':
	# test example
	import sys, os
	import numpy
	import scipy.stats

	seed = int(os.environ.get('SEED', '0'))
	#import pygsl.rng

	#rng = pygsl.rng.mt19937()
	#rng.set(seed)
	numpy.random.seed(seed)
	
	points = []
	ndim = 1

	def allocate():
		i = len(points)
		print 'allocate: assigning %d' % i
		points.append(None)
		return i

	def drawFromPrior(i):
		print 'drawFromPrior: %d' % i, points[i]
		points[i] = numpy.random.uniform(0, 1, size=ndim)
		#points[i] = rng.uniform()
		print 'drawFromPrior: drew', points[i]
		return 0

	def perturb(i):
		#print 'perturb: %d %.5f' % (i, points[i])
		params = points[i]
		# alter a random direction
		which = numpy.random.randint(ndim)
		p = params[which]
		#rng.set(1)
		#a = rng.uniform()
		#b = rng.gaussian(1.)
		#p += 10.**(1.5 - 6 * a) * b
		p += 10**(1.5 - 6 * numpy.random.uniform(0,1)) * numpy.random.normal(0, 1);
		# wrap around:
		#print 'perturb: before wrapping %.5f %.5f %.5f' % (p, a, b)
		points[i][which] = p - numpy.floor(p)
		H = 0
		#print 'perturb: new point %d %.5f' % (i, points[i])
		return H

	rv = scipy.stats.norm([0.654321]*ndim, [0.01]*ndim)
	def likelihood(i):
		params = points[i]
		l = rv.logpdf(params).sum()
		print 'likelihood: L = %.5f for %.5f' % (l, params)
		if numpy.random.randint(200) == 199:
			print 'likelihood: %d %-5.4f %-5.2f' % (i, points[i][0], float(l))
		return l
	
	def user_print(i):
		return b'%15f 1 ' % points[i]
	dnest_run(allocate=allocate, drawFromPrior=drawFromPrior, perturb=perturb, likelihood=likelihood,
		user_print=user_print, seed=seed)



