#ifndef CMD_C
#define CMD_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "matsolve.h"

#include "extern.c"

int exec_cmd(char *str,int len)
{
	if(db)printf("\nexec cmd called for %s, l=%d",str,len);
	printf("\n");
	trunc_str(&str,&len);
	if(len>0)
	{
		char *p=strchr(str,':'),*tmp,**args=(char **)malloc(8*sizeof(char *));
		char *s=str;
		if(p!=strrchr(str,':'))
		{
			printf("\nERROR\n multiple occurances of ':' found");
			return 0;
		}
		if(p!=0)
		{
			int l=p-s,arg_c;
			int (*cmd_mtch)(char *, int);
			trunc_str(&s,&l);
			tmp=(char *)malloc(l*sizeof(char));
			void *cmd_search;
			strncpy(tmp,s,l);
			arg_c=parse_ssv(tmp,args);
			cmd_search=search_node(cmd_tree,args[0]);
			cmd_mtch = cmd_search ? (int(*)(char *, int))cmd_search : 0;
			if(cmd_mtch)
			{
				if(db)printf("\nfn matched");
				cmd_mtch(p+1,len-(p-str)-1);
			}
			else if(arg_c==1)
			{
				var *var_tmp=add_var(args[0]);
				if(!var_tmp->addr)var_tmp->addr=( node *)malloc(sizeof( node));
				store_fn(p+1,len-(p-str)-1,var_tmp->addr);
			}
			return 1;
		}
		else
		{
			store_fn( str, len, fnc );
			printf("\n-----------------------------------------------\n");
				printf("\nThe soln is %lf",fnc->method->eval_v(fnc));
			printf("\n-----------------------------------------------\n");
			return 1;
		}
	}
	return 0;
}
int exec_cmd_mat(char *t,int l_t)
{
	node n,n1;
	trunc_str(&t,&l_t);
	store_fn(t,l_t,&n);
	n.method->eval_n(&n,&n1);
	if(n1.node_type==ar_mat)
	{
		printf("\n-----------------------------------------------\n");
			print_mat(n1.d.mat_ptr);
		printf("\n-----------------------------------------------\n");
	}
	return 0;
}

int exec_cmd_simpson(char *t,int l_t)
{
	//sm
	char **args=(char **)malloc(8*sizeof(char *));
	int c;
	trunc_str(&t,&l_t);
	parse_args(t,l_t,args,&c);
	printf(
		"\nIntegrating (%s)d%s using Simpson's rule from %s to %s using %s nodes:\n-\t-\t-\n"
		,args[0],args[1],args[2],args[3],args[4]
		);
	node n,a,b;
	store_fn(args[0],strlen(args[0]),&n);
	store_fn(args[2],strlen(args[2]),&a);
	store_fn(args[3],strlen(args[3]),&b);
	double result = simpson(&n,add_var(args[1]),a.method->eval_v(&a),b.method->eval_v(&b),atoi(args[4]));
	if(v)printf("\nInteration (%s)d%s (%lf-%lf)=%lf",args[0],args[1],a.method->eval_v(&a),b.method->eval_v(&b),result);
	printf("\n-\t-\t-\n");
	return 0;
}
int exec_cmd_simpson3(char *t,int l_t)
{
	//sm3
	char **args=(char **)malloc(8*sizeof(char *));
	int c;
	trunc_str(&t,&l_t);
	parse_args(t,l_t,args,&c);
	printf(
		"\nIntegrating (%s)d%s using simpson 3/8th rule from %s to %s using %s nodes:\n-\t-\t-\n"
		,args[0],args[1],args[2],args[3],args[4]
		);
	node n,a,b;
	store_fn(args[0],strlen(args[0]),&n);
	store_fn(args[2],strlen(args[2]),&a);
	store_fn(args[3],strlen(args[3]),&b);
	double result = simpson3(&n,add_var(args[1]),a.method->eval_v(&a),b.method->eval_v(&b),atoi(args[4]));
	if(v)printf("\nInteration (%s)d%s (%lf-%lf)=%lf",args[0],args[1],a.method->eval_v(&a),b.method->eval_v(&b),result);
	printf("\n-\t-\t-\n");
	return 0;
}
int exec_cmd_RK(char *t,int l_t)
{
	//RK
	char **args=(char **)malloc(8*sizeof(char *));
	int c;
	trunc_str(&t,&l_t);
	parse_args(t,l_t,args,&c);
	printf("solving initial value problem\n-\t-\t-\n");
	node f,x0,y0,x1;
	store_fn(args[0],strlen(args[0]),&f);
	store_fn(args[3],strlen(args[3]),&x0);
	store_fn(args[4],strlen(args[4]),&y0);
	store_fn(args[5],strlen(args[5]),&x1);
	double result = RK	(
		&f, add_var(args[1]), add_var(args[2]),x0.method->eval_v(&x0),
		y0.method->eval_v(&y0),x1.method->eval_v(&x1),atoi(args[6])
		);
	printf("\n%s(%f)=%f",args[2],x1.method->eval_v(&x1),result);
	printf("\n-\t-\t-\n");
	return 0;
}
int exec_cmd_newton(char *t,int l_t)
{
	//nwt
	char **args=(char **)malloc(8*sizeof(char *));
	int c;
	trunc_str(&t,&l_t);
	parse_args(t,l_t,args,&c);
	node n,z;
	store_fn(args[0],strlen(args[0]),&n);
	store_fn(args[2],strlen(args[2]),&z);
	double result = newton(&n,add_var(args[1]),z.method->eval_v(&z));
	printf("\n-----------------------------------------------\n");
	printf("%s=%f",args[1],result);
	printf("\n-----------------------------------------------\n");
	return 0;
}
int exec_cmd_plot(char *t,int l_t)
{
	char **args=(char **)malloc(8*sizeof(char *));
	int c;
	trunc_str(&t,&l_t);
	parse_args(t,l_t,args,&c);
	node n,a,b;
	store_fn(args[0],strlen(args[0]),&n);
	store_fn(args[2],strlen(args[2]),&a);
	store_fn(args[3],strlen(args[3]),&b);
	plot(&n,add_var(args[1]),a.method->eval_v(&a),b.method->eval_v(&b),atoi(args[4]));
	//printf("\n-----------------------------------------------\n");
	//printf("%s=%f",args[1],result);
	//printf("\n-----------------------------------------------\n");
	return 0;
}
int exec_cmd_eval(char *t,int l_t)
{
	trunc_str(&t,&l_t);
	char *tmp=(char*)malloc(l_t*sizeof(char));
	strcpy(tmp,t);
	 var *var_ptr=add_var(tmp);
	printf("\n\t-----\n%s = %lf\n\t-----\n",tmp,var_ptr->addr->method->eval_v(var_ptr->addr));
	return 0;
}
int match_cmd(char *f)
{
	switch (*f++)
	{
		case 'e':
			return (*f=='v' && *(f+1)=='a' && *(f+2)=='l' && *(f+3)=='\0') ? cmd_evl:0;
		case 'n':
			return (*f=='w' && *(f+1)=='t' && *(f+2)=='\0') ? cmd_nwt : 0;
		case 'R':
			return (*f=='K' && *(f+1)=='\0') ? cmd_RK:0;
		case 's':
			return (*f=='m' && *(f+1)=='3' && *(f+2)=='\0') ? cmd_sm3:0;
		default:
			return 0;
	}
	return 0;
}
#endif
