slatkin-exact-tools
===================

Command line tools and Python module for Montgomery Slatkin's "Exact" test of neutrality in a set of gene or trait counts

Author:  Mark E. Madsen, Dept. of Anthropology, University of Washington


### Goal ###

This package is a refactoring of Montgomery Slatkin's [code for an "Exact" test for neutrality](http://ib.berkeley.edu/labs/slatkin/monty/Ewens_exact.program).  

The refactoring allows the code to be used as a library, in addition to the original use as a console/command line program.  The original "enumerate" code is unchanged, and preserved for testing against modifications that I make to the monte carlo version.  

The monte carlo version has been migrated into `slatkin.h` and `slatkin.c`, with a single "API" function to call: `slatkin_mc()`, which takes the number of monte carlo replications, and an array of integers representing the allele/trait counts.  The function returns a simple struct with members `probability`, and the `theta_estimate`.

There is also a C language function called `montecarlo()` which wraps `slatkin_mc()`, taking a slightly different set of arguments.  This is meant specifically to be easy for SWIG to construct Python and other language modules from, but it might also be useful in C itself given that it takes `int**` instead of `int[]`.  

The remainder of the original functions are located in slatkin.c, and are considered "private" -- since C can't enforce that, just don't use them.  Many of them have interesting conventions for the arguments, and I have not rewritten any of this code, just refactored it into packages and added scaffolding.  

### Output ###

Slatkin's original output format for the monte carlo program has been changed to output two numbers, separated by a tab.  The first number is the P_e probability for the observed configuration given the Ewens Sampling Formula, and the second is the estimate of "theta" given equation 9.26 in Ewens's 1979 book (the equation is also in Ewens 2004, of course, but with a different equation number which escapes me for the moment.)

This modification is designed to allow easy scripting of Slatkin Exact tests, since the STDOUT results are "clean" and can be easily parsed by the next step in a script.  I've used this method to access Slatkin tests from within R as well, with good results.  

### Python Module ###

The build process constructs a Cython-based extension module, wrapping the C language `montecarlo()` function, and constructing a wrapper module.  The python implementation works as follows:

```python

from slatkin import ewens_montecarlo

# Do 100K monte carlo replicates
mc_reps = 100000 

counts = [91, 56, 27, 9, 7, 3, 1]

(prob, theta) = ewens_montecarlo(mc_reps, counts)

print "prob: %s    theta: %s" % (prob, theta)

```


The python module is constructed most easily by running `make python` or `make all`, and is installed by running `make pyinstall`.  The latter will install the built module to the user's current Python distribution, as determined by the python interpreter in the user's path.  In some cases, you may need `sudo make pyinstall` to make this work, or to make the module available for all users on the system.  


### Compatibility with Version 1.2 and Previous Revisions ###

Prior to Version 1.3, I was using SWIG to generate the Python to C wrapper code and frankly, the typemaps for getting the list in and out are confusing, so I took the easy way out and passed the length of the list
as a parameter to the `montecarlo()` function.  

In Version 1.3, I rewrote the wrapper using Cython, which is a really fabulous system.  Not only is the complexity vastly reduced, but once the OS X clang compiler handles OpenMP again, I'm set for parallelizing 
the monte carlo replicates across cores, which will help in situations with many trait counts and large numbers of replicates (which you'd want with many traits to get a stable answer).  

The former syntax is used in a number of my running simulation codes, though, so for compatibility, the old calling convention is:

```python

from slatkin import montecarlo
 
counts = [91, 56, 27, 9, 7, 3, 1]

(prob, theta) = montecarlo(mc_reps, counts, len(counts))

print "prob: %s    theta: %s" % (prob, theta)
```


### Random Number Generator ###

I also noted that Slatkin was implementing his own PRNG in the original code.  Today, there's no need to implement our own, and potentially get it wrong.  Instead, I'm building in the Mersenne Twister MT19937 as implemented in `mersenne.c`, which carries the following attribution legend:

```
// The code as Shawn received it included the following notice:
//
//   Copyright (C) 1997 Makoto Matsumoto and Takuji Nishimura.  When
//   you use this, send an e-mail to <matumoto@math.keio.ac.jp> with
//   an appropriate reference to your work.
//   It would be nice to CC: <Cokus@math.washington.edu> when you write.

```

### Memory Management ###

Slatkin's original code was meant to be run once against a set of data, at which point the process exits.  It was therefore unsuitable for use as a library, to be linked into a long running program.  None of the heap memory was ever freed, so you'd leak memory like a sieve using this against 100,000 or 1MM samples.  

This has been fixed, which means that it should be possible to write R or Python extensions to wrap the `slatkin-common` files.  This will be done in this repository in upcoming releases.  

### License ###

Given that Slatkin put no explicit license in his code, and seemingly placed it in the public domain, I have considered it OK to re-mix and refactor his code in this manner, and re-release the result.  I am attaching an GNU General Public License (version 3.0) to this repository and code, to ensure that others will be able to use the code, and pay forward Slatkin's generosity in releasing the code in the first place.  This also matches the license given in the Mersenne Twister code.  





