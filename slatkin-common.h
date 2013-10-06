#ifndef SLATKIN_COMMON
#define SLATKIN_COMMON

typedef struct {
	double probability;
	double theta_estimate;
} slatkin_result;



#define NR_END 1

#define A 16807
#define M 2147483647
#define Q 127773
#define R 2836

#define RM 2147483647.0


/* Main API function, should be called from outside */

slatkin_result slatkin_mc(int maxreps, int r_obs[]);


/* Declaration of internal methods */

void generate(int k, int n, int *r, double *ranvec, double **b);
void print_config(int k, int *r);
double ewens_stat(int *r);
double F(int k, int n, int *r);
double theta_est(int k_obs, int n);
double kval(double x, int n);
double **matrix(long nrl, long nrh, long ncl, long nch);
double *vector(long nl, long nh);
int *ivector(long nl, long nh);
void gsrand(int s);
double unif();
int grand();

double** create2DDoubleArray(int rows, int cols);

#endif