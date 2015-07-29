

from libc.stdlib cimport malloc, free

cdef extern from "slatkin_impl.h":

    ctypedef struct slatkin_result:
        double probability
        double theta_estimate

    slatkin_result slatkin_mc(int numreps, int[] obs)
    #slatkin_result slatkin_mc_parallel(int numreps, int[] obs)


def montecarlo(int num_reps, list obs_list, int compat_numalleles):
    """
    Compatibility interface with old SWIG wrapper. The numalleles parameter is ignored since
    the new code calculates the length of the list.

    :param num_reps: Number of monte carlo iterations
    :param obs_list:  Python list with trait counts
    :param compat_numalleles: for compatibility with old SWIG wrapper interface
    :return:
    """
    cdef int padded_numalleles
    cdef int parallel = 0

    padded_numalleles = len(obs_list) + 2
    result = _montecarlo(num_reps, obs_list, padded_numalleles, parallel)
    return (result.probability, result.theta_estimate)


def ewens_montecarlo(int num_reps, list obs_list):
    """
    New interface to slatkin test, just takes replicate number and a list of trait counts

    :param num_reps:
    :param obs_list:
    :return:
    """
    cdef int padded_numalleles
    cdef int parallel = 0

    padded_numalleles = len(obs_list) + 2
    result = _montecarlo(num_reps, obs_list, padded_numalleles, parallel)
    return (result.probability, result.theta_estimate)


# def ewens_montecarlo_parallel(int num_reps, list obs_list):
#     """
#     Calls a task parallel version of the monte carlo Slatkin test, which uses OpenMP
#     to distribute replicates across available cores.  This is useful mainly if you've got a
#     large set of counts (number of traits or alleles) and are doing a large number of replicates
#
#     :param num_reps:
#     :param obs_list:
#     :return:
#     """
#     cdef int padded_numalleles
#     cdef int parallel = 1
#
#     padded_numalleles = len(obs_list) + 2
#     result = _montecarlo(num_reps, obs_list, padded_numalleles, parallel)
#     return (result.probability, result.theta_estimate)



cdef slatkin_result _montecarlo(int numreps, list obs_list, int numalleles, int parallel):
    cdef int *obs = <int*>malloc(numalleles * sizeof(int))
    cdef int i = 0
    cdef int count

    obs[0] = 0
    i += 1

    for count in obs_list:
        obs[i] = count
        i += 1

    obs[i] = 0

    # if parallel == 1:
    #     result = slatkin_mc_parallel(numreps, obs)
    # else:
    result = slatkin_mc(numreps, obs)

    free(obs)
    return result

