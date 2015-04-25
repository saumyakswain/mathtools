#ifndef NODE_C
#define NODE_C

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "matsolve.h"

#include "extern.c"

int get_ab(node *,double *,double *);
int get_a(node *n,double *a);
int get_n1n2(node *,node *, node *);
extern int v,db;


//Function definitions
int get_n1n2(node *n,node *n1, node *n2)
{
	n->d.fn_ptr.node1->method->eval_n(n->d.fn_ptr.node1,n1);
	n->d.fn_ptr.node2->method->eval_n(n->d.fn_ptr.node2,n2);
	return 1;
}

int get_ab(node *n,double *a,double *b)
{
	node *n1=n->d.fn_ptr.node1, *n2=n->d.fn_ptr.node2;
	*a=n1->method->eval_v(n1);
	*b=n2->method->eval_v(n2);	
return 1;
}
int get_a(node *n,double *a)
{
	node *n1=n->d.fn_ptr.node1;
	*a=n1->method->eval_v(n1);
return 1;
}

node *n_add(node *n, node *t)
{
	if(db)printf("\nadd called");
	node n1,n2,*np1=n->d.fn_ptr.node1,*np2=n->d.fn_ptr.node2;
	np1->method->eval_n(np1,&n1);
	np2->method->eval_n(np2,&n2);
	int t1=n1.node_type,t2=n2.node_type;
	if(t1==cn_num && t2==cn_num)
	{
		t->node_type=cn_num;
		t->method=&m_cnn;
		t->d.value=n1.d.value+n2.d.value;
		return t;
	}
	else if(t1==ar_mat && t2==ar_mat)
	{
		if(n1.d.mat_ptr->c==n2.d.mat_ptr->c && n1.d.mat_ptr->r==n2.d.mat_ptr->r)
		{
			t->node_type=ar_mat;
			t->d.mat_ptr=mat_add(n1.d.mat_ptr,n2.d.mat_ptr);
			return t;
		}
		else
		{
			if(v)printf("\nERROR!\nunequal dimension of matrices");
			return 0;
		}
	}
	
	return 0;
}
node *n_sub(node *n, node *t)
{
	if(db)printf("\nsub called");
	node n1,n2,*np1=n->d.fn_ptr.node1,*np2=n->d.fn_ptr.node2;
	np1->method->eval_n(np1,&n1);
	np2->method->eval_n(np2,&n2);
	int t1=n1.node_type,t2=n2.node_type;
	if(t1==cn_num && t2==cn_num)
	{
		t->node_type=cn_num;
		t->method=&m_cnn;
		t->d.value=n1.d.value-n2.d.value;
		return t;
	}
	else if(t1==ar_mat && t2==ar_mat)
	{
		if(n1.d.mat_ptr->c==n2.d.mat_ptr->c && n1.d.mat_ptr->r==n2.d.mat_ptr->r)
		{
			t->node_type=ar_mat;
			t->d.mat_ptr=mat_sub(n1.d.mat_ptr,n2.d.mat_ptr);
			return t;
		}
		else
		{
			if(v)printf("\nERROR!\nunequal dimension of matrices");
			return 0;
		}
	}
	return 0;
}
node *n_mul(node *n, node *t)
{
	if(db)printf("\nmul called");
	node n1,n2,*np1=n->d.fn_ptr.node1,*np2=n->d.fn_ptr.node2;
	np1->method->eval_n(np1,&n1);
	np2->method->eval_n(np2,&n2);
	int t1=n1.node_type,t2=n2.node_type;
	
	if(t1==cn_num && t2==cn_num)
	{
		updt_num_node(t,n1.d.value*n2.d.value);
		return t;
	}
	else if(t1==ar_mat && t2==ar_mat)
	{
		if(n1.d.mat_ptr->c==n2.d.mat_ptr->r)
		{
			t->node_type=ar_mat;
			t->d.mat_ptr=mat_mul(n1.d.mat_ptr,n2.d.mat_ptr);
			return t;
		}
		else
		{
			if(v)printf("\nERROR!\ncolumn count of MAT 1 doesn't match row count of MAT 2");
			return 0;
		}
	}
	return 0;
}
node *n_div(node *n, node *t)
{
	if(db)printf("\ndiv called");
	node n1,n2,*np1=n->d.fn_ptr.node1,*np2=n->d.fn_ptr.node2;
	np1->method->eval_n(np1,&n1);
	np2->method->eval_n(np2,&n2);
	int t1=n1.node_type,t2=n2.node_type;
	
	if(t1==cn_num && t2==cn_num)
	{
		updt_num_node(t,n1.d.value/n2.d.value);
		return t;
	}
	return 0;
}
node *n_pow(node *n, node *t)
{
	if(db)printf("\nn_pow called");
	node n1,n2,*np1=n->d.fn_ptr.node1,*np2=n->d.fn_ptr.node2;
	np1->method->eval_n(np1,&n1);
	np2->method->eval_n(np2,&n2);
	int t1=n1.node_type,t2=n2.node_type;
	
	if(t1==cn_num && t2==cn_num)
	{
		updt_num_node(t,pow(n1.d.value,n2.d.value));
		return t;
	}
	return 0;
}
node *n_der(node *n, node *t)
{
	if(db)printf("\nder called");
	node *n1=n->d.fn_ptr.node1, *n2=n->d.fn_ptr.node2,n3;
	int t1=n1->node_type,t2=n2->node_type;
	if(db)printf("\nn::ntype:%d,t::ntype:%d",t1,t2);
	if(n1->method->der_n)
	{
		if(db)printf("\nder_n present");
	if(t1==cn_num)
	{
		return 0;
	}
	else if(t2==fn_var) 
	{
		if(db)printf("\nt2==fn_var");
		n1->method->der_n(n1,n2->d.var_ptr,&n3);
		n3.method->eval_n(&n3,t);
		return t;
	}
	}else printf("\neval_n absent");
	return 0;
}
node *n_cnn(node *n, node*t)
{
	*t=*n;
	return t;
}
node *n_mat(node *n, node*t)
{
	*t=*n;
	return t;
}

node *n_var(node *n, node*t)
{
	n->d.var_ptr->addr->method->eval_n(n->d.var_ptr->addr,t);
	return t;
}

node *n_err(node *n, node *t)
{
	if(v)printf("\nERROR! ILLEGAL OPERATION\nThis functionality is not defined");
	t->node_type=cn_num;
	t->method=&m_cnn;
	t->d.value=0;
	return t;
}
node *n_def(node *n, node *t)
{
	printf("\nThis functionality is not not defined yet. We regret the inconvenience");
	t->node_type=cn_num;
	t->method=&m_cnn;
	t->d.value=0;
	return t;
}

node *n_dr_add(node *n,var *x,node *t)
{
	t->d.fn_ptr.node1=(node *)malloc(sizeof(node));
	t->d.fn_ptr.node2=(node *)malloc(sizeof(node));
	t->node_type=op_plus;
	t->method=&m_add;
	n->d.fn_ptr.node1->method->der_n(n->d.fn_ptr.node1,x,t->d.fn_ptr.node1);
	n->d.fn_ptr.node2->method->der_n(n->d.fn_ptr.node1,x,t->d.fn_ptr.node2);
	return 0;
}
node *n_dr_sub(node *n,var *x,node *t)
{
	t->d.fn_ptr.node1=(node *)malloc(sizeof(node));
	t->d.fn_ptr.node2=(node *)malloc(sizeof(node));
	t->node_type=op_minus;
	t->method=&m_sub;
	n->d.fn_ptr.node1->method->der_n(n->d.fn_ptr.node1,x,t->d.fn_ptr.node1);
	n->d.fn_ptr.node2->method->der_n(n->d.fn_ptr.node1,x,t->d.fn_ptr.node2);
	return 0;
}
node *n_dr_var(node *n,var *x,node *t)
{
	if(db) printf("n_dr_var called\n");
	if(n->d.var_ptr==x)
	{
		t->node_type=cn_num;
		t->d.value=1;
		t->method=&m_cnn;
	}
	return t;
}
double d_add(node *n)
{
	if(db)printf("\nd_add called");
	double a,b;
	if(!get_ab(n,&a,&b))return 0;
	return a+b;
}
double d_sub(node *n)
{
	if(db)printf("d_sub called");
	double a,b;
	if(!get_ab(n,&a,&b))return 0;
	return a-b;
}
double d_mul(node *n)
{
	if(db)printf("\nd_mul called");
	double a,b;
	if(!get_ab(n,&a,&b))return 0;
	return a*b;
}
double d_div(node *n)
{
	if(db)printf("\nd_div called");
	double a,b;
	if(!get_ab(n,&a,&b))return 0;
	return a/b;
}
double d_pow(node *n)
{
	if(db)printf("\nd_exp called");
	double a,b;
	if(!get_ab(n,&a,&b))return 0;
	return pow(a,b);
}
double d_sin(node *n)
{
	double a;
	return get_a(n,&a)?sin(a):0;
}
double d_cos(node *n)
{
	double a;
	return get_a(n,&a)?cos(a):0;
}
double d_tan(node *n)
{
	double a;
	return get_a(n,&a)?tan(a):0;
}
double d_asin(node *n)
{
	double a;
	return get_a(n,&a)?asin(a):0;
}
double d_acos(node *n)
{
	double a;
	return get_a(n,&a)?acos(a):0;
}
double d_atan(node *n)
{
	double a;
	return get_a(n,&a)?atan(a):0;
}
double d_sec(node *n)
{
	double a;
	return get_a(n,&a)?1/sin(a):0;
}
double d_cosec(node *n)
{
	double a;
	return get_a(n,&a)?1/sin(a):0;
}
double d_cot(node *n)
{
	double a;
	return get_a(n,&a)?1/tan(a):0;
}
double d_asec(node *n)
{
	double a;
	return get_a(n,&a)?acos(1/a):0;
}
double d_acosec(node *n)
{
	double a;
	return get_a(n,&a)?asin(1/a):0;
}
double d_acot(node *n)
{
	double a;
	return get_a(n,&a)?atan(1/a):0;
}
double d_sinh(node *n)
{
	double a;
	return get_a(n,&a)?sinh(a):0;
}
double d_cosh(node *n)
{
	double a;
	return get_a(n,&a)?cosh(a):0;
}
double d_tanh(node *n)
{
	double a;
	return get_a(n,&a)?tanh(a):0;
}
double d_asinh(node *n)
{
	double a;
	return get_a(n,&a)?asinh(a):0;
}
double d_acosh(node *n)
{
	double a;
	return get_a(n,&a)?acosh(a):0;
}
double d_atanh(node *n)
{
	double a;
	return get_a(n,&a)?atanh(a):0;
}
double d_exp(node *n)
{
	double a;
	return get_a(n,&a)?exp(a):0;
}
double d_ln(node *n)
{
	double a;
	return get_a(n,&a)?log(a):0;
}
double d_log(node *n)
{
	double a;
	return get_a(n,&a)?log10(a):0;
}
double d_cnn(node *n)
{
	return n->d.value;
}
double d_var(node *n)
{
	if (n->d.var_ptr->addr) return n->d.var_ptr->addr->method->eval_v(n->d.var_ptr->addr);
	else
	{
		printf("\n\nERROR %s not defined",n->d.var_ptr->name);
		return 0;
	}
	return 0;
}
double d_mat(node *n)
{
	if(v)printf("\nERROR!\nIllegal operation on matrix"
				"\nCan't Return compound data type, returning zero");
	return 0;
}
double d_der(node *n)
{
	printf("\nder called");
	node *n1=n->d.fn_ptr.node1, *n2=n->d.fn_ptr.node2;
	int t1=n1->node_type,t2=n2->node_type;
	printf("\nn1::ntype:%d,n2::ntype:%d",t1,t2);
	if(n1->method->der_v)
	{
		printf("\nder_v present");
		if(t2==fn_var)
		{
			printf("\nt2==fn_var");
			
			if(t1==cn_num)return 0;
			else return n1->method->der_v(n1,n2->d.var_ptr);
		}
		else
		{
			printf("\ncan not find der wrt a const...");
			return 0;	//
		}
	}

	else printf("\nder_v absent");
	return 0;
}

double d_dr_cnn(node *n, var *x)
{
	return 0;
}
double d_dr_add(node *n, var *x)
{
	node *n1=n->d.fn_ptr.node1, *n2=n->d.fn_ptr.node2;
	return n1->method->der_v(n1,x)+n2->method->der_v(n2,x);
}
double d_dr_sub(node *n, var *x)
{
	node *n1=n->d.fn_ptr.node1, *n2=n->d.fn_ptr.node2;
	return n1->method->der_v(n1,x)-n2->method->der_v(n2,x);
}
double d_dr_mul(node *n, var *x)
{
	node *n1=n->d.fn_ptr.node1, *n2=n->d.fn_ptr.node2;
	return
	(n1->method->der_v(n1,x))*(n2->method->eval_v(n2))+
	(n2->method->der_v(n2,x))*(n1->method->eval_v(n1));
}
double d_dr_div(node *n, var *x)
{
	node *n1=n->d.fn_ptr.node1, *n2=n->d.fn_ptr.node2;
	return
	((n1->method->der_v(n1,x))*(n2->method->eval_v(n2))
	-(n2->method->der_v(n2,x))*(n1->method->eval_v(n1)))
	/pow((n1->method->eval_v(n1)),2);
}
double d_dr_pow(node *n, var *x)
{
	node *n1=n->d.fn_ptr.node1, *n2=n->d.fn_ptr.node2;
	if(db)printf("\nd_dr_pow called");
	if(n1->node_type==cn_num)
	{
		return (log(n1->d.value))*(n->method->eval_v(n))*
		(n2->method->der_v(n2,x));
	}
	else if(n2->node_type==cn_num)
	{
		return
		(n2->d.value)*
		(pow(n1->method->eval_v(n1),n2->d.value-1))*
		(n1->method->der_v(n1,x));
	}
	return 0;
}
double d_dr_sin(node *n, var *x)
{
	if(db)printf("\nd_dr_sin called");
	double a;
	return get_a(n,&a) ? cos(a)*n->d.node_ptr->method->der_v(n->d.node_ptr,x) : 0;
}
double d_dr_cos(node *n, var *x)
{
	if(db)printf("\nd_dr_cos called");
	double a;
	return get_a(n,&a) ? -1*sin(a)*n->d.node_ptr->method->der_v(n->d.node_ptr,x) : 0;
}
double d_dr_tan(node *n, var *x)
{
	if(db)printf("\nd_dr_tan called");
	double a;
	return get_a(n,&a) ? pow(1/cos(a),2)*n->d.node_ptr->method->der_v(n->d.node_ptr,x) : 0;
}
double d_dr_ln(node *n, var *x)
{
	if(db)printf("\nd_dr_ln called");
	double a;
	return get_a(n,&a) ? (1/a)*n->d.node_ptr->method->der_v(n->d.node_ptr,x) : 0;
}
double d_dr_log(node *n, var *x)
{
	if(db)printf("\nderivarive of log not defined");
	if(db)printf("\nd_dr_log called");
	double a;
	return get_a(n,&a) ?0 : 0;
}
double d_dr_exp(node *n, var *x)
{
	if(db)printf("\nd_dr_exp called\n");
	double a;
	return get_a(n,&a) ? (exp(a))*n->d.node_ptr->method->der_v(n->d.node_ptr,x) : 0;
}

double d_dr_var(node *n, var *x)
{
	if(db)printf("\nd_dr_var called\n");
	if(n->d.var_ptr==x)
	{
		return 1;
	}
	else
	{
		if(n->d.var_ptr->addr)
		{
			if(n->d.var_ptr->addr->method)
			return n->d.var_ptr->addr->method->der_v(n->d.var_ptr->addr,x);
			else
			{
				printf("\nnode method not found for %s",n->d.var_ptr->name);
				return 0;
			}
		}
		else
		{
			printf("\n\nERROR %s not defined",n->d.var_ptr->name);
			return 0;
		}
	}
	return 0;
}



#endif
