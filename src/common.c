#ifndef COMMON_C
#define COMMON_C

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "matsolve.h"

#include "extern.c"

int trunc_str(char **s,int *l)
{
	while(**s==' '|| **s=='\n' || **s == '\t')
	{
		(*s)++; (*l)--;
	}
	while(*(*s+*l-1)==' ' || *(*s+*l-1)=='\n' || *(*s+*l-1) == '\t')
	{
		(*l)--;
	}
	return 0;
}
int get_fn(char **s)
{
	register int i=0;
	char fnStr[2048];
	while(i<2044)
	{
		fnStr[i]=getc(stdin);
		if(fnStr[i]==';')break;
		if(fnStr[i]==EOF)
		{
			i=1;fnStr[0]='q';
			if(db)printf("\nEnd of file");break;
		}
		if(i && fnStr[i]=='/' && fnStr[i-1]=='/')
		{
			i=i-2;
			while(getc(stdin)!='\n')
			{
				
			}
		}
		i++;
	}
	fnStr[i]='\0';
	int l=strlen(fnStr);
	char *s1=fnStr;
	trunc_str(&s1,&l);
	if(*s1=='/' && *(s1+1)=='/')return get_fn(s);
	*s=(char *)malloc(l*sizeof(char));
	strcpy(*s,s1);
	return strlen(*s);
}
int validate_fn(char *f, int length)
{
	if(db)printf("\nvalidate_fn() called");
	register int i=0;
	for(;i<length;i++)
	{
		if(f[i]==';'||f[i]=='\''||f[i]=='='||f[i]=='"')	
		{
			return 0;
		}
	}
	
	return 1;
}
int store_fn(char *s, int length,  node *n)
{
	int i;
	if(db)
	{
		printf("\nstore_fn() called for");
		for(i=0;i<length;i++)printf("%c",*(s+i));
		printf("\n");
	}
	trunc_str(&s,&length);
	
	if(!bin_op('+',op_plus,&m_add,s,length,n))
	{
		if(!bin_op('-',op_minus,&m_sub,s,length,n))
		{
			if(!bin_op('*',op_asterick,&m_mul,s,length,n))
			{
				if(!bin_op('/',op_slash,&m_div,s,length,n))
				{
					if(!bin_op('^',op_pow,&m_pow,s,length,n))
					{
						if(!bin_op('#',op_der,&m_der,s,length,n))
						{
							if(!store_num(s,length,n))
							{
								if(*s=='+'|| *s=='-')
								{
									s++; length--;
									if(*(s-1)=='-')
									{
										node *n1=(node *)malloc(sizeof(node));
										node *n2=(node *)malloc(sizeof(node));
										n->node_type=op_asterick;
										n->d.fn_ptr.node1=n1;
										n->d.fn_ptr.node2=n2;
										n->method=&m_mul;
										updt_num_node(n1,-1);
										store_fn(s,length,n2);
										return 0;
									}
								}
								if(*s=='$' && *(s+1)!='$')
								{
									node *n_t=(node *)malloc(sizeof(node));
									store_fn(s+1,length-1,n_t);
									updt_num_node(n,n_t->method->eval_v(n_t));
									return 0;
								}
								if(*s=='$' && *(s+1)=='$')
								{
									node *n_t=(node *)malloc(sizeof(node));
									store_fn(s+2,length-2,n_t);
									n_t->method->eval_n(n_t,n);
									return 0;
								}
								if(!parse_fn(s,length,n))
								{
									if(*s=='(' && *(s+length-1)==')')
									store_fn(s+1,length-2,n);
								}
							}
						}
					}
				}
			}
		}
	}

	return 0;
}
int bin_op(char op,int node_type,methods *met,char *s,int length, node *n)
{
	if(db)printf("\nbin_op called for %c method:%p",op,met);
	int brace_count=0;
	if(*(s+length-1)==op)
	{
		printf("\n!!!!!\nsyntax error at %d\n!!!!!",length);
		
	}	
	int tmp,i_min=0;
	
	register int i;
	
	if(*s==op)
	{
		if(op=='+' || op=='-')
		i_min=1;
		else printf("\n!!!\nsyntax error\n!!!");
	}
	for(i=length-1;i>=i_min;i--)
	{
		tmp=chk_brace(s+i);
		brace_count+=tmp;
		if(tmp!=0)continue;
		if(brace_count==0)
		{
			if(*(s+i)==op)
			{
				//assign node type
				n->node_type=node_type;
				//create new nodes
				node *node1=(node *)malloc(sizeof(node));
				node *node2=(node *)malloc(sizeof(node));
				//assign pointers
				n->d.fn_ptr.node1=node1;
				n->d.fn_ptr.node2=node2;
				n->method=met;
				store_fn(s,i,node1);
				store_fn(s+i+1,length-i-1,node2);
				optimise_bin_op_node(n);
				if(n->node_type==op_pow && n->d.fn_ptr.node1->node_type != cn_num && n->d.fn_ptr.node2->node_type != cn_num)
				{
					if(db)printf("\nwill convert to exp:: n1_typ:%d, n2_typ:%d",n->d.fn_ptr.node1->node_type,n->d.fn_ptr.node2->node_type);
					if(n->d.fn_ptr.node1->node_type == fn_var && n->d.fn_ptr.node1->d.var_ptr == add_var("e"))
					{
						if(db)printf("\ne^ found");
						free(node1);
						n->node_type = fn_exp;
						n->method = &m_exp;
						n->d.node_ptr = node2;
					}
					else
					{
						printf("\nf(x)^(x) form");
						node *node3=(node *)malloc(sizeof(node));
						node *node4=(node *)malloc(sizeof(node));
						node3->node_type=fn_ln;
						node3->method=&m_ln;
						node3->d.node_ptr=n->d.fn_ptr.node1;
						node4->node_type=op_asterick;
						node4->method=&m_mul;
						node4->d.fn_ptr.node1=n->d.fn_ptr.node2;
						node4->d.fn_ptr.node2=node3;
						n->node_type=fn_exp;
						n->method=&m_exp;
						n->d.node_ptr=node4;
					}
					optimise_fn_node(n);
				}
				return 1;
			}
		}
	}
	return 0;
}
int store_num(char *s,int length, node *n)
{
	if(db)printf("\nstore_num called");
	/*register int i=0;
	char a[32];
	if(*s=='+' || *s=='-')
	{
		if(*(s+1)<'9' && *(s+1)>'0')
		{
			if(*s=='-')
			{
				if(db)printf("\n'-' found");
				a[0]=*s; i++;
			}
		}else return 0;
	}
	while(((*(s+i)>='0' && *(s+i)<='9') || *(s+i)=='.') && i<31 && i<length)
	{
		a[i]=*(s+i);
		i++;
	}
	a[i]='\0';*/
	char *e;
	double a=strtod(s,&e);
	if(e == s) return 0;
	if(e-s < length)
	{
		node *n1=( node *)malloc(sizeof( node));
		node *n2=( node *)malloc(sizeof( node));
		n->node_type=op_asterick;
		n->method=&m_mul;
		n->d.fn_ptr.node1=n1;
		n->d.fn_ptr.node2=n2;
		updt_num_node(n1,a);
		store_fn(e,length-(e-s),n2);
		return 1;
	}
	else
	{
		updt_num_node(n,a);
		return 1;
	}
	return 0;
}
int parse_fn(char *s,int length, node *n)
{
	
	if(*s=='$')s++;
	register int i=0;
	char tmp_fn[18];
	fn_data *tmp_mtch;
	char *args[32];
	int arg_c;
	while(i<length && *(s+i)!='(' && *(s+i)!=' ')
	{
		tmp_fn[i]=*(s+i);
		i++;
	}
	if(i==0) return 0;
	tmp_fn[i]='\0';
	char *tmp_fn_ptr=(char *)malloc((i+1)*sizeof(char));
	strcpy(tmp_fn_ptr,tmp_fn);
	tmp_mtch=(fn_data *)search_node(fn_tree,tmp_fn_ptr);
		if(tmp_mtch)
		{
			parse_args(s+i,length-i,args,&arg_c);
			n->node_type=tmp_mtch->fn_type;
			n->method=tmp_mtch->method;
			node *node1=(node *)malloc(sizeof( node));
			if(arg_c)
			{
				switch (tmp_mtch->fn_type)
				{
					case ar_mat:
						if(arg_c==3)return store_mat(args,n);
						else return arg_err(tmp_fn_ptr,arg_c,3,n);
					default:
						if(arg_c>1)return arg_err(tmp_fn_ptr,arg_c,1,n);
						else
						{
							n->d.node_ptr=node1;
							store_fn(s+i,length-i,node1);
							optimise_fn_node(n);
							return 1;
						}
				}
			}
			
			else return arg_err(tmp_fn_ptr,arg_c,1,n);
		}
		else
		{
			n->node_type=fn_var;
			n->method=&m_var;
			n->d.var_ptr=add_var(tmp_fn_ptr);//changed
			return 1;
		}
		
	return 0;
}
int match_fnc(char *f)
{
	if(db)printf("mtch fn called for: '%s', len:%d\n",f,(int)strlen(f));
	switch (*f++)
	{
		case 'a':
			switch (*f++)
			{
				case 's':
				switch (*f++)
				{
					case 'i':
						return (*f=='n' && *(f+1)=='\0') ?fn_sin:0;
					case 'e':
						return (*f=='c' && *(f+1)=='\0') ?fn_sec:0;
					default:
						return 0;
				}
				case 'c':
					switch(*f++)
					{
						case 'o':
						switch (*f++)
						{
							case 's':
							switch (*f++)
							{
								case '\0':
									return fn_cos;
								case 'e':
									return (*f=='c' && *(f+1)=='\0') ? fn_cosec:0;
								default:
									return 0;
							}
							case 't':
								return (*f=='\0') ? fn_cot:0;
							default:
								return 0;
						}
						default:
							return 0;
					}
			case 't':
			return (*f=='a' && *(f+1)=='n' && *(f+2)=='\0') ? fn_tan : 0;
	
			default:
			return 0;
		}
		
		case 'c':
		switch(*f++)
		{
			case 'o':
			switch (*f++)
			{
				case 's':
				switch (*f++)
				{
					case '\0':
					return fn_cos;
					case 'e':
					return (*f=='c' && *(f+1)=='\0') ? fn_cosec :0;
					default:
					return 0;
				}
				case 't':
				return (*f=='\0') ? fn_cot:0;
				default:
				return 0;
			}
			default:
			return 0;
		}
		case 'd':
			return (*f=='e' && *(f+1)=='t' && *(f+2)=='\0') ? fn_det : 0;
			
		case 'e':
			return (*f=='x' && *(f+1)=='p' && *(f+2)=='\0') ? fn_exp : 0;
			
			
		case 'l':
			switch (*f++)
			{
				case 'o':
					return (*f=='g' && *(f+1)=='\0') ? fn_log:0;
				case 'n':
					return (*f=='\0') ? fn_ln:0;
				default:
					return 0;
			}
		
		case 'm':
			return (*f=='a' && *(f+1)=='t' && *(f+2)=='\0') ? ar_mat:0;
		
			
		case 's':
		switch (*f++)
		{
			case 'i':
				return (*f=='n' && *(f+1)=='\0') ? fn_sin:0;
			case 'e':
				return (*f=='c' && *(f+1)=='\0') ? fn_sec:0;
			
			default:
			return 0;
		}
		
		case 't':
			return (*f=='a' && *(f+1)=='n' && *(f+2)=='\0') ? fn_tan:0;
		
		default:
			return 0;
	}
}

double modulus(double x)
{
	if(x>=0)return x;
	else return -x;
}
double d(var *y, char *x_str)
{
	if(db)printf("\nd called");
	var *x= add_var(x_str);
	return differentiate(y->addr,x->addr);
}
double differentiate(node *y, node *x)
{
	if(db)printf("\ndiff called for %p#%p , %d#%d",y,x,y->node_type,x->node_type);
	switch (y->node_type)
	{
		case op_plus:
			return differentiate(y->d.fn_ptr.node1,x)+differentiate(y->d.fn_ptr.node2,x);
		case op_minus:
			return differentiate(y->d.fn_ptr.node1,x)-differentiate(y->d.fn_ptr.node2,x);
		case op_asterick:
			return evaluate(y->d.fn_ptr.node1)*differentiate(y->d.fn_ptr.node2,x)+evaluate(y->d.fn_ptr.node2)*differentiate(y->d.fn_ptr.node1,x);
		case op_slash:
			return (evaluate(y->d.fn_ptr.node1)*differentiate(y->d.fn_ptr.node2,x)-evaluate(y->d.fn_ptr.node2)*differentiate(y->d.fn_ptr.node1,x))/pow(evaluate(y->d.fn_ptr.node2),2);
		case op_pow:
			return evaluate(y->d.fn_ptr.node2)*pow(evaluate(y->d.fn_ptr.node1),evaluate(y->d.fn_ptr.node2)-1);
		case op_der:
			return differentiate(y->d.fn_ptr.node1,y->d.fn_ptr.node2);
		case fn_exp:
			return exp(evaluate(y->d.node_ptr))*differentiate(y->d.node_ptr,x);
		case fn_sin:
			return cos(evaluate(y->d.node_ptr))*differentiate(y->d.node_ptr,x);
		case fn_cos:
			return (-1)*sin(evaluate(y->d.node_ptr))*differentiate(y->d.node_ptr,x);
		case fn_var:
			if(y->d.var_ptr->addr==x)
			{
				return 1;
			}
			else
			{
				return differentiate(y->d.var_ptr->addr,x);
			}
		case cn_num:
			return 0;
		default:
			return 0;
	}
	return 0;
}
int arg_err(char *f,int c,int r, node *n)
{
	printf("\nERROR\n'%s' expects %d arguments, %d arguments supplied\nPlease re-enter the function\n",f,r,c);
	char *t;
	int l=get_fn(&t);
	store_fn(t,l,n);
	return 1;
}

var *add_var(char *v_nm)
{
	var *tmp_var;
	if((tmp_var = (var *)search_node(var_tree,v_nm)))
	{
		//do nothin
	}
	else
	{
		tmp_var = ( var*)malloc(sizeof( var));
		tmp_var->name = v_nm;
		tmp_var->addr = 0;
		push_node(&var_tree, v_nm, tmp_var);
	}
	return tmp_var;
	return 0;
}
int chk_brace(char *pos)
{
	switch (*pos)
	{
		case '(': return 1;
		case ')': return -1;
		default	: return 0;
	}
}
int parse_args(char *s,int l, char **args, int *arg_c)
{
	int a=0,br_c=0,i=0,j=0,k=0,tmp;
	char tmp_arg[256];
	if(db)
	{
		printf("\nparse_args called for\n");
		for(;a<l;a++)printf("%c",*(s+a));
		printf("\nl=%d\n",l);
	}
	trunc_str(&s,&l);
	if(*s=='('&&*(s+l-1)==')')
	{
		s++; l=l-2;
	}
	trunc_str(&s,&l);
	while(i<l)
	{
		tmp_arg[j]=*(s+i);
		tmp=chk_brace(s+i);
		br_c+=tmp;
		if((br_c==0 && *(s+i)==',') || i==l-1)
		{
			if(i==l-1) j++;
			tmp_arg[j]='\0';
			args[k]=(char *)malloc((j+1)*sizeof(char));
			strncpy(args[k],tmp_arg,j);
			j=-1;k++;
			if(db)printf("\nk: %d",k);
		}
		i++;j++;
	}
		

	args[k]=0;
	
	i=0;
	if(db)
	{
		while(args[i]!=0){printf("\narg%d:%s",i,args[i]);i++;}
		printf("\nk=%d",k);
	}
		*arg_c=k;
		return k;
}
double evaluate(node *node_ptr)
{
	if(db)printf("\nevaluate called for %d",node_ptr->node_type);
	switch(node_ptr->node_type)
	{
		case op_plus:
			return evaluate(node_ptr->d.fn_ptr.node1)+evaluate(node_ptr->d.fn_ptr.node2);
		case op_minus:
			return evaluate(node_ptr->d.fn_ptr.node1)-evaluate(node_ptr->d.fn_ptr.node2);
		case op_asterick:
			return evaluate(node_ptr->d.fn_ptr.node1)*evaluate(node_ptr->d.fn_ptr.node2);
		case op_slash:
			return evaluate(node_ptr->d.fn_ptr.node1)/evaluate(node_ptr->d.fn_ptr.node2);
		case op_pow:
			return pow(evaluate(node_ptr->d.fn_ptr.node1),evaluate(node_ptr->d.fn_ptr.node2));
		case op_der:
			return differentiate(node_ptr->d.fn_ptr.node1->d.var_ptr->addr,node_ptr->d.fn_ptr.node2->d.var_ptr->addr);
		case cn_num:
			return node_ptr->d.value;
		case fn_sin:
			return sin(evaluate(node_ptr->d.node_ptr));
		case fn_cos:
			return cos(evaluate(node_ptr->d.node_ptr));
		case fn_tan:
			return tan(evaluate(node_ptr->d.node_ptr));
		case fn_asin:
			return asin(evaluate(node_ptr->d.node_ptr));
		case fn_acos:
			return acos(evaluate(node_ptr->d.node_ptr));
		case fn_atan:
			return atan(evaluate(node_ptr->d.node_ptr));
		case fn_sec:
			return 1/cos(evaluate(node_ptr->d.node_ptr));
		case fn_cosec:
			return 1/sin(evaluate(node_ptr->d.node_ptr));
		case fn_cot:
			return 1/tan(evaluate(node_ptr->d.node_ptr));
		case fn_asec:
			return 1/acos(evaluate(node_ptr->d.node_ptr));
		case fn_acosec:
			return 1/asin(evaluate(node_ptr->d.node_ptr));
		case fn_acot:
			return 1/atan(evaluate(node_ptr->d.node_ptr));
		case fn_exp:
			return exp(evaluate(node_ptr->d.node_ptr));
		case fn_ln:
			return log(evaluate(node_ptr->d.node_ptr));
		case fn_log:
			return log10(evaluate(node_ptr->d.node_ptr));
		case fn_var:
			if(node_ptr->d.var_ptr->addr) return evaluate(node_ptr->d.var_ptr->addr);
			else{printf("\n%s not initialised: please initialise it first",node_ptr->d.var_ptr->name); return 0;}
		case fn_det:
			//if(node_ptr->d.node_ptr->node_type==ar_mat)return det(node_ptr->d.node_ptr->d.mat_ptr);
			//else
			{
				//printf("\nERROR\nCan't find determinant of a non-matrix\n");
			}
			return 0;
		default:
			return 0;
	}
}

node *eval(node *n)
{
	if(db)printf("eval called for %d\n",n->node_type);
	node *t=(node *)malloc(sizeof(node));
	switch(n->node_type)
	{
		case op_plus:
			return n_add(n,t);
		case op_minus:
			return n_sub(n,t);
		case op_asterick:
			return n_mul(n,t);
		case fn_var:
			return n->d.var_ptr->addr;
		default:
			//t->node_type=cn_num;
			//t->d.value=evaluate(n);
			//return t;
			return 0;
	}
	return 0;
}
int parse_ssv(char *s, char **val_ary)
{
	if(db)printf("parsessv called for\n%s\n",s);
	int l=strlen(s),i=0,j=0,k=0,tmp,br_c=0;
	trunc_str(&s,&l);
	char tmp_arg[256];
	while(i<l)
		{
			tmp_arg[j]=*(s+i);
			tmp=chk_brace(s+i);
			br_c+=tmp;
			if((br_c==0 &&(*(s+i)==' ' || *(s+i)=='\n' ||*(s+i)=='\t'))|| i==l-1)
			{
					if(i==l-1) j++;
					tmp_arg[j]='\0';
					val_ary[k]=(char *)malloc((j+1)*sizeof(char));
					strncpy(val_ary[k],tmp_arg,j);
					j=-1;k++;
					while(*(s+i+1)==' ' || *(s+i+1)=='\n' ||*(s+i+1)=='\t'){ i++;}
			}
			i++;j++;
		}
		val_ary[k]=0;
		//printf(" k: %d\n",k);
		return k;
}
int store_mat(char **args,  node *n)
{
	int r=atoi(args[0]);
	int c=atoi(args[1]);
	int i=0,v_c;
	char **m_v_str=(char **)malloc(r*c*sizeof(char *)+1);
	double *mat_vals=(double *)malloc(r*c*sizeof(double));
	if(db)printf("\nstoremat called for\nrow=%d, col=%d",r,c);
	if((v_c=parse_ssv(args[2],m_v_str))==r*c)
	{
		while(i<r*c && m_v_str[i]!=0)
		{
			mat_vals[i]=strtof(m_v_str[i],0);
			if(db)printf("%lf\t",mat_vals[i]);
			i++;
		}
		mat *mat_p=(mat *)malloc(sizeof(mat));
		n->d.mat_ptr=mat_p;
		mat_p->r=r;
		mat_p->c=c;
		mat_p->addr=mat_vals;
		return 1;
	}
	else if(v_c)
	{
		printf("\nERROR\nMore values expected in matrix\nsupplied nos=%d, required=%d\n",v_c,r*c);
		return 0;
	}
	else return 0;
}
int optimise_bin_op_node(node *n)
{
	if(n->d.fn_ptr.node1->node_type==cn_num && n->d.fn_ptr.node2->node_type==cn_num)
	{
		double a=n->method->eval_v(n);
		free(n->d.fn_ptr.node1);
		free(n->d.fn_ptr.node2);
		updt_num_node(n,a);
	}
	return 0;
}
int optimise_fn_node(node *n)
{
	if(n->d.node_ptr->node_type==cn_num )
	{
		double a=n->method->eval_v(n);
		free(n->d.node_ptr);
		updt_num_node(n,a);
	}
	return 0;
}
int updt_num_node(node *n, double value)
{
	n->node_type = cn_num;
	n->method = &m_cnn;
	n->d.value = value;
	return 0;
}
int plot(node *n, var *x, double a, double b, int N)
{
	x->addr=(node *)malloc(sizeof(node));
	updt_num_node(x->addr,a);
	double h= (b-a)/N;
	int i;
	for(i=0;i<=N;i++)
	{
		printf("\nx:%lf\ty:%lf",x->addr->d.value,n->method->eval_v(n));
		x->addr->d.value += h;
	}
	return 0;
}
#endif
