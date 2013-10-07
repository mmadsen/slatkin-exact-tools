/*  
 This program implements a Monte Carlo algorithm to perform both the 
 homozygosity and exact tests on a sampling of alleles.  It uses an 
 algorithm devised by Frank Stewart to generate random configurations 
 from the Ewens sampling distribution.
 
 To use the program, you need to list the number of copies of each allele as 
 elements of the vector r_obs listed just below the beginning of the main 
 program below.  The data do not have to be in decreasing order, as they 
 do for the program that enumerates all configurations.
 
 Two sample data sets are included.  The first is the sample data set used in
 the 1994 paper in Genetical Research (64: 71-74).  This data set can also be 
 analyzed using ENUMERATE.  The second data set contains the numbers of the 
 24 most frequent cystic fibrosis alleles in a sample of CF cases in northern 
 Europe.  This data set is much too large to be analyzed using the other 
 program.  The second data set should produce the output
 
 n = 16975, k = 24, theta = 2.681168, F = 0.768561, maxrep = 100000
 P_E(approx) = 0.28207
 P_H(approx) = 0.99802
 
 It took about 25 minutes on a 100 mhz HP workstation for 100,000 replicates.
 
 To use a data set, remove the / and * symbols that indicate a comment 
 from one of the r_obs.
 
 The program produces two output values.  The first is the value of P_H, 
 the tail probability for the Ewens-Watterson test using homozygosity as 
 a test statistic.  The second is P_E, the tail probability for the exact 
 test.  For small data sets, including the first sample data set, the two 
 values are the same.  The 1994 paper was wrong in claiming otherwise and 
 that error was corrected in the 1996 paper in Genetical Research 
 {68: 259-260).  For larger data sets, the P values can differ, sometimes 
 substantially, as in the second sample data set.
 
 MONTE CARLO requires a random number seed (initseed), which you can change 
 by editing the file.  If you change initseed or the data set, you have 
 to recompile the program.  Once the program is compiled, you run it by 
 specifying the number of replicates on the command line.  Usually, 100,000 
 replicates is more than sufficient but you can try different values to 
 find out for yourself. 
 */

#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "slatkin.h"


#define min(x, y)  (((x) < (y)) ? x : y)


int main(int argc, char *argv[]) {
    

    
	/* Cramor Place decorated sherd counts: int r_obs[] = {0, 91,56,27,9,7,3,1, 0}; */
	/*  int r_obs[] = {0, 9, 2, 1, 1, 1, 1, 1, 0};  */
	/*int r_obs[] = {0, 30, 62, 97, 15, 53, 18, 55, 35, 57, 14866,
     160, 439, 18, 356, 165, 40, 41, 14, 27, 36, 39, 23, 120, 209, 0}; */
     //  CF alleles from North Europe: P_H = 0.9977   
	int r_obs[KLIMIT + 2];  	/*  observed configuration */
	int i,  k, maxrep;
	slatkin_result results;

    
	if (argc == 1)  {
		printf("Specify the number of replicates, and then the configuration on the command line\n");
		exit(0);
	}

	/* replicate the r_obs array that montecarlo is expecting, with zeros on both ends, but get it from the CL */

	r_obs[0] = 0;

	maxrep = atoi(argv[1]);
	k = argc - 2;
	for (i=1; i<=k; i++)  {
		r_obs[i] = atoi(argv[i + 1]);
	}
	r_obs[k + 1] = 0;

	/*
	k1 = argc - 1;
	printf("Configuration entered on command line: ");
	for (i = 0; i < k1; i++) {
		printf(" %i ", r_obs[i]);
	}*/

	results = slatkin_mc(maxrep, r_obs);
	
	printf("%g\t%g\n", results.probability, results.theta_estimate );

	
	return 0;	
}  /*  end, main  */
