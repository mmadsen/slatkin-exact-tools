#!/usr/bin/env python

from slatkin import montecarlo

counts = [8, 4, 3, 2, 2, 1, 1, 1, 1, 1, 1] 
numalleles = len(counts)

(prob,theta) = montecarlo(100000, counts, len(counts))

print "prob: %s    theta: %s" % (prob, theta)


counts2 = [91, 56, 27, 9, 7, 3, 1]

(prob, theta) = montecarlo(100000, counts2, len(counts2))

print "prob: %s    theta: %s" % (prob, theta)
