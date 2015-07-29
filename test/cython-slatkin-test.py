import slatkin

counts = [8, 4, 3, 2, 2, 1, 1, 1, 1, 1, 1]

(prob, theta) = slatkin.montecarlo(100000, counts)
print "prob: %s    theta: %s" % (prob, theta)

counts = [30, 62, 97, 15, 53, 18, 55, 35, 57, 14866,160, 439, 18, 356, 165, 40, 41, 14, 27, 36, 39, 23, 120, 209]
(prob, theta) = slatkin.montecarlo(100000, counts)
print "prob: %s    theta: %s" % (prob, theta)


