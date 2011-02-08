// subs.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "subs.h"

static double* pparticles;           /* Particle x, y coordinates    */
static double* pparticlesnew;        /* Updated particle coordinates */
static double* fparticles;           /* Interparticle forces         */
static double* vparticles;           /* Particle velocities          */
static double* prad;                 /* Particle radii               */
static double* pradstart;            /* Initial particle radii       */

static int numparticles;             /* Number of particles          */
static int rseed;                    /* Random seed                  */

static double springkrepel;          /* Global repulsive forces      */
static double stddevfac;             /* Std deviation factor         */
static double lowerprad;             /* Particle radius lower bound  */
static double upperprad;             /* Particle radius upper bound  */
static double particlerad;           /* Mean particle radius         */
static double pi = 3.14159265358979; /* Pi!                          */

/* Calculate index into "2D" array. */
int p_index(int dim, int particle)
{
  int index = (dim*numparticles) + particle;
  return index;
}

/* Return the greater of two values */
double max(double u, double v)
{
  if (u > v)
  {
    return u;
  }
  else
  {
    return v;
  }
}

/* Set simulation parameters, allocate arrays */
void initialise(int num_particles, int random_seed, int spring_krepel, 
                int std_dev_fac, int particle_radius)
{
  numparticles = num_particles;
  rseed = random_seed;
  springkrepel = spring_krepel;
  stddevfac = std_dev_fac;
  particlerad = particle_radius;

  /* particle_array_size needs to be big enough to hold 2D *
   * coordinates for each particle.                        */
  size_t particle_array_size = 2*num_particles*sizeof(double);
  size_t radius_array_size   =   num_particles*sizeof(double);

  pparticles    = (double*)malloc(particle_array_size);
  pparticlesnew = (double*)malloc(particle_array_size);
  fparticles    = (double*)malloc(particle_array_size);
  vparticles    = (double*)malloc(particle_array_size);

  prad      = (double*)malloc(radius_array_size);
  pradstart = (double*)malloc(radius_array_size);

  if (  (pparticles == NULL)
     || (pparticlesnew == NULL)
     || (fparticles == NULL)
     || (vparticles == NULL)
     || (prad == NULL)
     || (pradstart == NULL) )
  {
    fprintf(stderr, "Allocation error.\n");
    exit(EXIT_FAILURE);
  }
}

/* Free memory at the end of the simulation. */
void finalise()
{
  free(pparticles);
  free(pparticlesnew);
  free(fparticles);
  free(vparticles);
  free(prad);
  free(pradstart);
}

double normal_distribution()
{
  return 0.0;
}

void distribute_particles_randomly()
{
  int p, numparticlesold, iter;
  double theta, tabletdr;

  /* Seed the random number generator */
  srand48(rseed);

  /* Set upper and lower bounds on particle radius */
  lowerprad = max(0.0, (particlerad - 2.0*stddevfac*particlerad) );
  upperprad = particlerad + 2.0*stddevfac*particlerad;

  for (p=0; p<numparticles; ++p)
  {
    if (stddevfac>0)
    {
      double outputrad = normal_distribution();
      prad[p] = outputrad;
      pradstart[p] = outputrad;
    }
    else
    {
      prad[p] = particlerad;
      pradstart[p] = particlerad;
    }
  }

  /* Initialise particle array to 0 */
  for (p=0; p<numparticles; ++p)
  {
    pparticles[p_index(0,p)] = 0.0;
    pparticles[p_index(1,p)] = 0.0;
  }

  /* First particle is centred in the domain */
  pparticles[p_index(0,0)] = 0.0;
  pparticles[p_index(0,1)] = 0.0;

  /* Save the total number of particles.               */
  /* We only want to compute for the particles we have */
  numparticlesold = numparticles;
  numparticles = 1;

  /* Insert particles into the domain at the average */
  /* particle radius from the centre. (The mass of   */
  /* particles grows outwards).                      */
  for (p=1; p<numparticlesold; ++p)
  {
    theta = drand48();
    theta = theta*2.0*pi;
    tabletdr = particlerad;

    pparticles[p_index(0,p)] = cos(theta)*tabletdr;
    pparticles[p_index(1,p)] = sin(theta)*tabletdr;

    /* Let the system settle with the current number of */
    /* active particles.                                */
    numparticles = p;
    iter = particlepos(0.0, 0.01, 1.0);
    printf("Particle %d took %d iterations.\n", p, iter);
  }

  printf("Finished randomising particle positions.\n");
}

void output_positions(int file_index)
{

}

int particlepos(int grav_fac, int dt_fac, int min_threshold)
{
  int iterations = 0;

  return iterations;
}
