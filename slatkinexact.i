/*****************************
Swig module description file
for the slatkin-exact-tools.

Generate by:
swig -python slatkin-common.i

Mark E. Madsen
mark@madsenlab.org
Oct 2013
******************************/

%module slatkinexact

%{
#include "slatkin-common.h"
#include "mersenne.h"	
%}

extern slatkin_result slatkin_mc(int maxreps, int r_obs[]);
