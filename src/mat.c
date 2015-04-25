/*
 * mat_fn.c
 */
#ifndef MAT_C
#define MAT_C

#include <stdio.h>
#include <stdlib.h>

#include "matsolve.h"

#include "extern.c"

int swap_row(mat **A,int r1,int r2)
{
	int c=(*A)->c,i=0;
	double t,*M=(*A)->addr;
	for(;i<c;i++)
	{
		t=*(M+r1*c+i);
		*(M+r1*c+i)=*(M+r2*c+i);
		*(M+r2*c+i)=t;
	}
	return 0;
}
mat *eliminate(mat *B)                         // Function to form Upper triangular matrix
{
	 mat *A=(mat *)malloc(sizeof(mat));
	*A=*B;
	A->addr=(double *)malloc(sizeof((B->r)*(B->c)*sizeof(double)));
	int t=0,i,j,k,jmax,rmax=A->r,c=A->c;
	double m,max,*M=A->addr,*M1=B->addr;
	for(;t<rmax*c;t++)
	{
		*(M+t)=*(M1+t);
	}
	for(i=0;i<rmax;i++)
	{
		max=0;
		for(j=0;j<rmax;j++)
		{
			if(modulus(*(M+j*c+i))>modulus(max) && *(M+i*c+j)!=0)
			{
				max=*(M+j*c+i);
				jmax=j;
			}
		}
		if(modulus(max)>modulus(*(M+i*c+i))){swap_row(&A,i,jmax);if(v)printf("\nswap:R %d,R %d\n",i,jmax);}
		if(*(M+i*c+i)==0)printf("Can't solve, Rank less than %d Enter another eqn\n",rmax);

		for(j=i+1;j<rmax;j++)
		{
			m=(*(M+j*c+i))/(*(M+i*c+i));
			*(M+j*c+i)=0;
			for(k=i+1;k<rmax+1;k++)
			{
				*(M+j*c+k)=*(M+j*c+k)-m*(*(M+i*c+k));
			}
		}
		
	}
	return A;
}
mat *mat_sub(mat *A,mat *B)
{
	int Ar=A->r,Ac=A->c,Br=B->r,Bc=B->c;
	double *Aa=A->addr,*Ba=B->addr;
	if(Ar!=Br || Ac!=Bc)
	{
		if(v)printf("ERROR\nUnequal dimensions\n");
		return 0;
	}
	int i,j;
	double *M=(double *)malloc(Ar*Bc*sizeof(double));
	 mat *C=( mat *)malloc(sizeof( mat));
	C->r=Ar;
	C->c=Bc;
	for(i=0;i<Ar;i++)
	{
		for(j=0;j<Bc;j++)
		{
			*(M+i*Bc+j)+=(*(Aa+i*Ac+j))-(*(Ba+i*Bc+j));
		}
	}
	C->addr=M;
	return C;
}

mat *mat_add( mat *A, mat *B)
{
	int Ar=A->r,Ac=A->c,Br=B->r,Bc=B->c;
	double *Aa=A->addr,*Ba=B->addr;
	if(Ar!=Br || Ac!=Bc)
	{
		if(v)printf("ERROR\nUnequal dimensions\n");
		return 0;
	}
	int i,j;
	double *M=(double *)malloc(Ar*Bc*sizeof(double));
	 mat *C=( mat *)malloc(sizeof( mat));
	C->r=Ar;
	C->c=Bc;
	for(i=0;i<Ar;i++)
	{
		for(j=0;j<Bc;j++)
		{
			*(M+i*Bc+j)+=(*(Aa+i*Ac+j))+(*(Ba+i*Bc+j));
		}
	}
	C->addr=M;
	return C;
}
mat *mat_mul( mat *A, mat *B)
{
	
	int Ar=A->r,Ac=A->c,Br=B->r,Bc=B->c;
	if(db)printf("mat_mul called\nAr %d, Ac %d, Br %d, Bc %d\n",Ar,Ac,Br,Bc);
	double *Aa=A->addr,*Ba=B->addr;
	if(Ac!=Br)
	{
		if(v)printf("c1!=r2\n");
		return 0;
	}
	int i,j,k;
	double *M=(double *)malloc(Ar*Bc*sizeof(double));
	mat *C=(mat *)malloc(sizeof(mat));
	C->r=Ar;
	C->c=Bc;
	for(i=0;i<Ar;i++)
	{
		for(j=0;j<Bc;j++)
		{
			for(k=0;k<Ac;k++)
			{
				*(M+i*Bc+j)+=(*(Aa+i*Ac+k))*(*(Ba+k*Bc+j));
			}
		}
	}
	C->addr=M;
	if(db)printf("exiting mat_mul");
	return C;
}

int print_mat(mat *M)
{
	// mat *M=n->d.mat_ptr;
	double *m=M->addr;
	int r=M->r,c=M->c;
	//if(db)printf("print_mat called for node_type %d\nr=%d, c=%d\n",n->node_type,r,c);
	int i,j;
	//double sol=0;
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
			printf("%lf\t",*(m+i*c+j));
			
		}
		printf("\n");
	}
	return 0;
}

#endif
