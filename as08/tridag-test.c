#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int tridag(double adiag[], double alower[], double aupper[], double rrhs[], double xsoln[], int n)
{
  int j;
  double bet,*gam;

/* if the following happens rethink your equations */
  if (fabs(adiag[0]) == 0.0) return -1;    // Error

/* one vector of workspace is needed */
  gam = malloc(n*sizeof(double));

  bet=adiag[0];   // bet this is used as workspace
  xsoln[0]=rrhs[0]/bet;

  for (j=1;j<n;j++) {
    gam[j]=aupper[j-1]/bet;
    bet=adiag[j]-alower[j]*gam[j];
    if (fabs(bet) == 0.0) return -1;   // Pivoting Error
    xsoln[j]=(rrhs[j]-alower[j]*xsoln[j-1])/bet;
  }
  for (j=n-2;j>=0;j--) {
    xsoln[j]-= gam[j+1]*xsoln[j+1];  // Backsubstition
  }
  free(gam);
  return 0;
}

int main() {
 
  int ierr;
  int iindex;

  int nsize;
  nsize = 3;  // hard-coded size for example problem

  double adiag[nsize],alower[nsize],aupper[nsize]; //create matrix arrays
  double rrhs[nsize],xsoln[nsize]; //create right-hand-side and soln arrays

  adiag[0] = 7.0;
  adiag[1] = 7.0;
  adiag[2] = 7.0;
  alower[0] = 0.0;  // NOTE not used by tridag
  alower[1] = 3.0;
  alower[2] = 1.0;  
  aupper[0] = 3.0;
  aupper[1] = 1.0;
  aupper[2] = 0.0;  // NOTE not used by tridag
  rrhs[0] = 27.0;
  rrhs[1] = 24.0;
  rrhs[2] = 9.0;

  ierr = tridag(adiag,alower,aupper,rrhs,xsoln,nsize);
// ierr = tridag(adiag,alower,aupper,rrhs,xsoln,(nsize-1)); // NOTE not this
  
  if (ierr==0) {
    for (iindex=0; iindex<nsize; iindex++){
      printf("xsoln element[%d]= %f \n",iindex,xsoln[iindex]);
    }
  } else {
    printf("ierr=0 so no tridag solution found!\n");
    exit(-1);
  }

  return 0;
}

