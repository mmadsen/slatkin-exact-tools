#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include "slatkin-common.h"


static int seed;

/* Main External API */

slatkin_result slatkin_mc(int maxreps, int r_obs[]) {
	slatkin_result results;
	double theta_estimate;
	int i, j, k, n, repno, Ecount, Fcount;
	int *r_random, *r_random_to_free;
	double E_obs, F_obs;
	double *ranvec;

	gsrand(time(NULL));
	
	/* Find k and n from the observed configuration  */
	
	k = 0;
	n = 0;
	while (r_obs[k+1]) {
		k++;
		n+=r_obs[k];
    }

    /*
		memory management notes - the following are dynamically allocated and need to be freed:

		Slatkin allocates these in the ivector, vector, and matrix methods, BUT does not pass back
		the raw pointer to the allocated region or "head." Instead, he passes back the spot where he wants
		the calling code to write data (in the case of the vector call especially).  So...we have to readjust
		the pointers *back* in order to free them...wacky, but true.  

		r_random -- what's being returned is head + 1, so free r_random - 1
		ranvec -- just free this pointer
		b -- have to walk K+1 rows, free each of the rows, then free b
    */



	r_random = ivector(0, k+1);
	r_random_to_free = r_random - 1;

	r_random[0] = r_random[k+1] = 0;
	ranvec = vector(1, k-1);  // to avoid doing this in each replicate
	
	/*  fill b matrix  */
	
	double **b = matrix(1, k, 1, n);

	for (j=1; j<=n; j++)
		b[1][j] = 1.0 / j;
	for (i=2; i<=k; i++)  {
		b[i][i] = 1.0;
		for (j=i; j<n; j++)
			b[i][j+1] = (i * b[i-1][j] + j * b[i][j]) / (j + 1.0);
    }
    
	F_obs = F(k, n, r_obs);
	E_obs = ewens_stat(r_obs);
	/*printf("\nn = %d, k = %d, theta = %g, F = %g, maxrep = %d\n",
           n, k, theta_est(k, n), F_obs, maxrep);*/
	Ecount = 0;
	Fcount = 0;
	for (repno=1; repno<=maxreps; repno++)  {
		generate(k, n, r_random, ranvec, b);
		if (ewens_stat(r_random) <= E_obs) 
			Ecount++;
		if (F(k, n, r_random) <= F_obs)
			Fcount++;
    }

    theta_estimate = theta_est(k, n); 

    results.probability = (double) Ecount / (double) maxreps;
    results.theta_estimate = theta_estimate;

    /* free the dynamically allocated memory - the matrix b is still problematic */
    free(ranvec);
    free(r_random_to_free);



    /* 
		the allocations occur in the matrix() function, starting on line 207

		The matrix first allocates a number of rows (k) as pointers to doubles.  
		then it allocates all the columns (n) as pointers to doubles.  
		So to unwind this, we need to walk the matrix and free each row of doubles, and then we 
		can free the original list of double* pointers to the rows themselves.  

		right?  
    */

	// columns are n+2 because ncol in matrix = n+1, and then he adds NR_END = 1 to it.  
	int col_size = (n+2) * sizeof(double);
	double **row_ptr = b;
	for(i=1; i <= k+1; i++) {
		//free(*row_ptr);
		row_ptr += col_size;
	}

    free(b);

    return results;
}



/* Internal Methods */

void generate(int k, int n, int *r, double *ranvec, double **b)  {  
	double unif(), cum;
	int i, l, nleft;
	
	for (i=1; i<=k-1; i++)
		ranvec[i] = unif();
	nleft = n;
	for (l=1; l<k; l++)  {
		cum = 0.0;
		for (i=1; i<=nleft; i++) {
			cum += b[k-l][nleft-i] / (i * b[k-l+1][nleft]);
			if (cum >= ranvec[l]) break;
        }
		r[l] = i;
		nleft -= i;
    }
	r[k] = nleft;
}

void print_config(int k, int *r) {
	int i;
    
	printf("(");
	for (i=1; i<k; i++)
		printf("%d,", r[i]);
	printf("%d)", r[k]);
	printf("\n");
}

double ewens_stat(int *r)  {
	int *ipt;
	double coef;
    
	coef = 1.0;
	for (ipt=r+1; *ipt; ipt++)
		coef *= *ipt;
	return 1.0 / coef;
}

double F(int k, int n, int *r)  {
    int i;
    double sum;
    
    sum = 0.0;
    for (i=1; i<=k; i++)  sum += r[i] * r[i];
    return sum / (n * n);
}

double theta_est(int k_obs, int n)  {
    /*  Estimates theta = 4N*mu using formula 9.26 in Ewens' book  */
	double kval(double theta, int n);
	double xlow, xhigh, xmid;
	double eps;
	
	eps = 0.00001;
	xlow = 0.1;
	while (kval(xlow, n) > k_obs)
		xlow /= 10.0;
	xhigh = 10.0;
	while (kval(xhigh, n) < k_obs)
		xhigh *= 10.0;
	while ((xhigh - xlow) > eps)  {
		xmid = (xhigh + xlow) / 2.0;
		if (kval(xmid, n) > k_obs)
			xhigh = xmid;
		else
			xlow = xmid;
    }
	return xmid;
}  /*  end, theta_est  */

double kval(double x, int n)  {
	int i;
	double sum;
	
	sum = 0.0;
	for (i=0; i<n; i++)
		sum += x / (i + x);
	return sum;
}



double **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;
	void nrerror(char error_text[]);
    
	

	// allocate pointers to rows 
	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;
    
	// allocate rows and set pointers to them 
	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl; 
    
	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
    
	/* return pointer to array of pointers to rows */
	return m;
}

double *vector(long nl, long nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
	double *v;
	void nrerror(char error_text[]);
	
	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

int *ivector(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
	int *v;
	void nrerror(char error_text[]);
    
	v=(int *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(int)));
	if (!v) nrerror("allocation failure in ivector()");
	return v-nl+NR_END;
}

void nrerror(char error_text[])
{
	fprintf(stderr,"Run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}


void gsrand(s)
int s;
{
	seed = s;
}



double unif()  /* This is drand renamed to be consistent with my usage  */
{
	int grand();
	return ((double) grand() / RM);
}

int grand()
{
	int test;
	
    test = A * (seed % Q) - R * (seed / Q);
    if (test > 0) seed = test;
    else seed = test + M;
    return(seed);
}