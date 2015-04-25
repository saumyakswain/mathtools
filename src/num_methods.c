
#ifndef NUM_METHODS_C
#define NUM_METHODS_C
#include <stdio.h>
#include <stdlib.h>


#include "matsolve.h"

#include "extern.c"
double newton(node *n,var *x, double z)                                             //funtion to calculate x by Newton's method
{
	//if(db)printf("newton called for '%s'#'%s'--'%lf'\n",y,x,z);
	double x1,tmp;
	int c=0,i=0;
	x->addr=(node *)malloc(sizeof(node));
	updt_num_node(x->addr,z);
	if(v)printf("\niteration %d :\t\t%lf",i,z);
	while(i<100 && c==0)
	{				
		tmp=n->method->der_v(n,x);
		if(tmp==0)
		{
			if(v)printf("\nDerivative zero at this point, incrementing x by 0.1\n"); 
			x->addr->d.value=x->addr->d.value+0.1; i--; c=1; continue;
		}
		else if(tmp<0.001 && tmp>-0.001)
		{
			if(v)printf("\nDerivative approaching zero, convergence will be slow or iteration may not terminate\n");
			c=1;
		}
		x1=x->addr->d.value-n->method->eval_v(n)/tmp;
		if(x->addr->d.value-x1<0.0000001 && x->addr->d.value-x1>-0.0000001)
		break;
		if(v)printf("\niteration %d :\t\t%lf",i+1,x1);
		x->addr->d.value=x1;
		i++;
	}
	if(i==100)
	if(v)printf("\nThe iteration may not terminate\n");
	return x->addr->d.value;
}
double RK(node *eq, var *x2, var *y2, double x0, double y0, double x1, int N)
{
    int i;
	double k1,k2,k3,k4,h=(x1-x0)/N;
	x2->addr=( node *)malloc(sizeof( node));
    x2->addr->node_type=cn_num;
    x2->addr->method=&m_cnn;
    y2->addr=( node *)malloc(sizeof( node));
    y2->addr->node_type=cn_num;
    y2->addr->method=&m_cnn;
	for(i=0;i<N;i++)
	{
		if(v)printf("\n%lf\t%lf",x0,y0);
		x2->addr->d.value=x0;
		y2->addr->d.value=y0;
		k1=h*eq->method->eval_v(eq);
		x2->addr->d.value=x0+h/2;
		y2->addr->d.value=y0+k1/2;
		k2=h*eq->method->eval_v(eq);
		x2->addr->d.value=x0+h/2;
		y2->addr->d.value=y0+k2/2;
		k3=h*eq->method->eval_v(eq);
		x2->addr->d.value=x0+h;
		y2->addr->d.value=y0+k3;
		k4=h*eq->method->eval_v(eq);
		x0=x0+h;
		y0=y0+(k1+2*k2+2*k3+k4)/6;
	}
	if(v)printf("\n%lf\t%lf",x0,y0);
	return y0;
}

double simpson(node *y,var *x,double a,double b,int n)
{
	if(v)printf("\na:%lf,b:%lf",a,b);
	x->addr=(node *)malloc(sizeof(node));
    x->addr->node_type=cn_num;
    x->addr->method=&m_cnn;
	int i;
	double h,s,s0=0,s1=0,s2=0,t=a;
	n += n%2;
	h=(b-a)/n;
	x->addr->d.value=a;
	s0+=y->method->eval_v(y);
	x->addr->d.value=b;
	s0+=y->method->eval_v(y);
	for(i=1;i<n;i++)
	{
		t+=h;
		x->addr->d.value=t;
		i%2 ? (s1+=y->method->eval_v(y)) : (s2+=y->method->eval_v(y));
	}
	
	
	s=h*(s0+4*s1+2*s2)/3;
	
	if(v)printf("\nThe value of the integration is: %lf",s);
	return s;
}
double simpson3(node *y,var *x,double a,double b,int n)
{
	if(v)printf("\na:%lf,b:%lf",a,b);
	x->addr=(node *)malloc(sizeof(node));
    x->addr->node_type=cn_num;
    x->addr->method=&m_cnn;
	int i;
	double h,s,s0=0,s1=0,s2=0,t=a;
	n += (3-(n%3));
	h=(b-a)/n;
	x->addr->d.value=a;
	s0+=y->method->eval_v(y);
	x->addr->d.value=b;
	s0+=y->method->eval_v(y);
	for(i=1;i<n;i++)
	{
		t+=h;
		x->addr->d.value=t;
		i%3==0 ? (s1+=y->method->eval_v(y)) : (s2+=y->method->eval_v(y));
	}
	
	
	s=h*3*(s0+2*s1+3*s2)/8;
	
	if(v)printf("\nThe value of the integration is: %lf",s);
	return s;
}
#endif //NUM_METHODS_C
