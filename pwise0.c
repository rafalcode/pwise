/* make a pairwise comparison table (PWCT) .. list out a table
 * where the index of the row represents the first of the pair, and each member of the row represents
 * the indices of the original sequence that they are compared with */
/* Actually derived from pwco3 in cwhats */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define OFFSET 1

typedef unsigned char boole;
typedef struct // cartpos
{
	float x,y;
} cartpos;  /* a cartesian position, to lend reality. */

typedef struct // ptdst point-distance
{
    int ab[2]; // the two points that we're comparing distance from a to b.
    float d;
} ptdst; // point distances.

int cmppdst(const void *ipd1, const void *ipd2)
{
    ptdst *pd1 = (ptdst*)ipd1;
    ptdst *pd2 = (ptdst*)ipd2;

    if(pd1->d > pd2->d)
        return 1;
    else if(pd1->d < pd2->d)
        return -1;
    else
        return 0;
}

void pretpi(ptdst *pwa, int n, int nr, int idx) // print distances for a particular index
{
    int i, j, mi, mj;
    int nc=nr; // it's always this, put in for clarity.
    int *p=calloc(nr, sizeof(int));
    mi=0;
    for(i=0;i<idx;++i) {
        p[i]=mi;
        printf("%i ", mi+idx-i-1); // took a while to get this, lots of fiddling involved.
        mi+=nr-i;
    }
    printf(" | "); 
    mj=nc-i; // gradually decreasing extent of the column run
    for(j=0;j<mj;++j) {
        printf("%i ", mi+j);
    }
    printf("\n"); 
    free(p);
}

ptdst **pdsti(ptdst *pwa, int n, int nr, int idx) // return ptdst points for a particular index
{
    int i, j, mi, mj, k;
    int nc=nr; // it's always this, put in for clarity.
    ptdst **pwf=malloc(nr*sizeof(ptdst*)); // these pointers to the pwa are the distances of this idx to all the other points.
    mi=0;
    for(i=0;i<idx;++i) {
        pwf[i] = pwa+mi+idx-i-1;
        mi+=nr-i;
    }
    k=i; // record it.
    mj=nc-i; // gradually decreasing extent of the column run
    for(j=0;j<mj;++j)
        pwf[k++] = pwa+mi+j;
    return pwf;
}

void pretp0(int n, int nr)
{
    int i, j, mi, mj;
    int nc=nr; // it's always this, put in for clarity.
    mi=0;
    for(i=0;i<nr;++i) {
        mj=nc-i; // gradually decreasing extent of the column run
        if(i>0) {
            printf("%i ", i-1);
            printf("%i ", mi-1);
        }
        for(j=0;j<mj;++j) {
            printf("%i ", mi+j);
        }
        printf("\n"); 
        mi+=n-i-1; // cumulative start position for the column run.
    }
}

void pretp(ptdst *pwa, int n, int nr)
{
    int i, j, mi, mj, k, kk;
    int nc=nr; // it's always this, put in for clarity.
    printf("Pretty print of pwa:\n"); 
    j=0;
    mi=0;
    printf("FROM:/TO: "); 
    for(i=1;i<n;++i)
        printf("p_%d    ", i); 
    printf("\n");
    for(i=0;i<nr;++i) {
        mj=nc-i; // gradually decreasing extent of the column run
        printf("p_%d:    ", i);
        for(k=0;k<i;++k) 
            for(kk=0;kk<7;++kk) 
                putchar(' '); // filler
        for(j=0;j<mj;++j) {
            printf("%2.4f ", pwa[mi+j].d);
        }
        printf("\n"); 
        mi+=n-i-1; // cumulative start position for the column run.
    }
}

ptdst *calcpwa(int n, cartpos *na, int npwc)
{

    int i, j, mi, mj;

    /* We're going to create a holder for the 1to1 relationships.*/
    ptdst *pwa=malloc(npwc*sizeof(ptdst));
    int nr=n-1; // number of rows our pairwise comp table will have
    int nc=n-1;
    printf("Total PWCT size: %i / Num rows: %i / Num cols: %d\n", npwc, nr, nc);

    mi=0;
    for(i=0;i<nr;++i) {
        mj=nc-i; // gradually decreasing extent of the column run
        for(j=0;j<mj;++j) {
            pwa[mi+j].d=sqrt(pow((na[i].x-na[i+j+1].x), 2)+pow((na[i].y-na[i+j+1].y), 2));
            pwa[mi+j].ab[0] = i;
            pwa[mi+j].ab[1] = i+j+1;
            printf("p%d/%d.vs.%d=%2.4f ", mi+j, pwa[mi+j].ab[0], pwa[mi+j].ab[1], pwa[mi+j].d);
        }
        printf("\n"); 
        mi+=n-i-1; // cumulative start position for the column run.
    }

    // for(i=0;i<n;++i) 
    //     pretpi(pwa, n, nr, i);
    // pretp0(n, nr);
    return pwa;
}

int main(int argc, char *argv[])
{
    if(argc!=2) {
        printf("Error. Pls supply argument (integer).\n");
        exit(EXIT_FAILURE);
    }

    int i;
    int n=atoi(argv[1]);

    /* OK, here we have our collection of elements that we wish to pair up in all possible combos */
    cartpos *na=calloc(n, sizeof(cartpos));
    printf("We printout the points, these are random points inside a unit square\n"); 
    for(i=0;i<n;++i) {
        na[i].x=(float)rand()/RAND_MAX;
        na[i].y=(float)rand()/RAND_MAX;
    }

    int npwc=n*(n-1)/2; // well known, from the maths.
    ptdst *pwa=calcpwa(n, na, npwc);
    pretp(pwa, n, n-1);
    // qsort(pwa, npwc, sizeof(ptdst), cmppdst);
    int ridx= 3; // random index I so happened to choose
    ptdst **pwf=pdsti(pwa, n, n-1, ridx);

    printf("Example distance array for indx no. %i\n", ridx); 
    for(i=0;i<n-1;++i) 
        printf("%2.4f ", pwf[i]->d);
    printf("\n"); 

    free(na);
    free(pwf);
    free(pwa);
    return 0;
}
