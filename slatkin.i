/*****************************
Swig module description file
for the slatkin-exact-tools.

Generate by:
swig -python slatkin-common.i

Mark E. Madsen
mark@madsenlab.org
Oct 2013
******************************/

%module slatkin
%include "typemaps.i"
%apply double *OUTPUT { double* probability, double* theta_estimate };


%typemap(in) int**  {
 /* Check if is a list */
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    int i = 0;
    $1 = (int **) malloc((size+1)*sizeof(int *));
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input,i);
      if (PyInt_Check(o))
		$1[i] = PyInt_AsLong(PyList_GetItem($input,i));
      else {
		PyErr_SetString(PyExc_TypeError,"list must contain ints");
		free($1);
		return NULL;
      }
    }
    $1[i] = 0;
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}

// This cleans up the char ** array we malloc'd before the function call
%typemap(freearg) int ** {
  free((int*) $1);
}

/*  

	the preceding allows us to call the method like this in python:

	from slatkin import montecarlo
	(prob,theta) = montecarlo(maxreps, count_list, numalleles)

*/


%{
#include "slatkin.h"
#include "mersenne.h"	
%}

extern void montecarlo(int maxreps, int** counts, int numalleles, double* probability, double* theta_estimate);
