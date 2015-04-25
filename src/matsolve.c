#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "matsolve.h"


//function prototypes

/*
 * 
 * global varriables
 * 
 */

methods 
	m_cnn={n_cnn,d_cnn,0,d_dr_cnn},m_mat={n_mat,d_mat,0,0},m_var={n_var,d_var,n_dr_var,d_dr_var},
	m_add={n_add,d_add,0,d_dr_add},m_sub={n_sub,d_sub,0,d_dr_sub},m_mul={n_mul,d_mul,0,d_dr_mul},
	m_div={n_div,d_div,0,0},m_pow={n_pow,d_pow,0,d_dr_pow},m_der={n_der,d_der,0,0},
	m_sin={0,d_sin,0,d_dr_sin},m_cos={0,d_cos,0,d_dr_cos},m_tan={0,d_tan},
	m_asin={0,d_asin},m_acos={0,d_acos},m_atan={0,d_atan,0,d_dr_tan},
	m_sec={0,d_sec},m_cosec={0,d_cosec},m_cot={0,d_cot},
	m_asec={0,d_asec},m_acosec={0,d_acosec},m_acot={0,d_acot},
	m_sinh={0,d_sinh},m_cosh={0,d_cosh},m_tanh={0,d_tanh},
	m_asinh={0,d_asinh},m_acosh={0,d_acosh},m_atanh={0,d_atanh},
	m_ln={0,d_ln,0,d_dr_ln},m_log={0,d_log,0,d_dr_log},m_exp={0,d_exp,0,d_dr_exp};
	
fn_data fn_data_arr[]=
{
	{"sin",fn_sin,&m_sin},{"cos",fn_cos,&m_cos},{"tan",fn_tan,&m_tan},
	{"asin",fn_asin,&m_asin},{"acos",fn_acos,&m_acos},{"atan",fn_atan,&m_atan},
	{"sec",fn_sec,&m_sec},{"cosec",fn_cosec,&m_cosec},{"cot",fn_cot,&m_cot},
	{"asec",fn_asec,&m_asec},{"cosec",fn_acosec,&m_acosec},{"cot",fn_acot,&m_acot},
	{"sinh",fn_sinh,&m_sinh},{"cosh",fn_cosh,&m_cosh},{"tanh",fn_tanh,&m_tanh},
	{"asinh",fn_asinh,&m_asinh},{"acosh",fn_acosh,&m_acosh},{"atan",fn_atanh,&m_atanh},
	{"ln",fn_ln,&m_ln},{"log",fn_log,&m_log},{"exp",fn_exp,&m_exp},
	{"mat",ar_mat,&m_mat}
	
};
cmd_data cmd_data_arr[]=
{
	{"EVAL",exec_cmd_eval},{"NEWTON",exec_cmd_newton},{"MAT",exec_cmd_mat},
	{"RK",exec_cmd_RK},{"SIMPSON",exec_cmd_simpson},{"SIMPSON_3/8",exec_cmd_simpson3},
	{"PLOT",exec_cmd_plot}
};

bt_str_node *fn_tree, *var_tree, *cmd_tree;
cmd_q *cmd_0,*cmd_q_pos;
node *fnc;
int v=0,db=0;
/*
 * 
 * var end
 * 
 */
 

void print_start_page()
{
	char c;
	FILE *start_pg=fopen("./mathserve/start.html","r");
	while((c=fgetc(start_pg))!=EOF ) putchar(c);
	
}
int start(int argc, char* argv[])
{
	//printf("argc_strt:%d  argv[1]:%s\n",argc,argv[1]);
	char par_set=0;
	int pipe_fds[2];
	int read_fd;
	int write_fd;
	pipe (pipe_fds);
	read_fd = pipe_fds[0];
	write_fd = pipe_fds[1];
	FILE *fpw=fdopen(write_fd,"w");
	//int par_cnt=0;
	if(argc>1)
	{
		if(strcmp(argv[1],"-v")==0)v=1;
		else if(strcmp(argv[1],"-d")==0)db=1;
		else if(strcmp(argv[1],"start")==0){print_start_page();return 0;}
		else {fprintf(fpw,"%s",argv[1]);fflush(fpw);par_set=1;}
	}
	if(argc>2)
	{
		if(strcmp(argv[2],"-v")==0)v=1;
		else if(strcmp(argv[2],"-d")==0)db=1;
		else {fprintf(fpw,"%s",argv[2]);fflush(fpw);par_set=1;}
	}
	if(argc>3)
	{
		if(strcmp(argv[3],"-v")==0)v=1;
		else if(strcmp(argv[3],"-d")==0)db=1;
		else {fprintf(fpw,"%s",argv[3]);fflush(fpw);par_set=1;}
	}
	if(par_set)	dup2(read_fd,STDIN_FILENO);
	//printf("%s",gets(t));
	m_init();
	register int i=1;
	cmd_q_pos=cmd_0;
	cmd_q *pr_pos;
	printf
	(
	"matsolve (c)SKS 2013 \n"
	"matsolve is a free software for solving mathematical problems involving\n"
	"numerical methods\n\n"
	);
	printf("Exit sequence is 'q;<enter>'\n\n");
	while(1)
	{
		if(v)printf(">");
		cmd_q_pos->cmd_len=get_fn(&(cmd_q_pos->cmd_str));
		if(cmd_q_pos->cmd_len==1 && cmd_q_pos->cmd_str[0]=='q') break;
		//store_fn(cmd_q_pos->cmd_str,cmd_q_pos->cmd_len,fnc);
		cmd_q_pos->next=(cmd_q *)malloc(sizeof(cmd_q));
		pr_pos=cmd_q_pos;
		cmd_q_pos=cmd_q_pos->next;
		cmd_q_pos->prev=pr_pos;
		cmd_q_pos->next=0;
		exec_cmd(pr_pos->cmd_str,pr_pos->cmd_len);
		fflush(stdin);
		i++;
	}
	fflush(stdin);
	//eliminate(add_var("A")->addr->d.mat_ptr);
	printf("\nbye!");
	return 0;
}
int m_init()
{
	int i;
	//initialising function tree
	for(i=0;i<sizeof(fn_data_arr)/sizeof(fn_data);i++)
	{
		push_node(&fn_tree, fn_data_arr[i].fn_name, fn_data_arr+i);
	}
	print_bt(fn_tree);
	for(i=0;i<sizeof(cmd_data_arr)/sizeof(cmd_data);i++)
	{
		push_node(&cmd_tree, cmd_data_arr[i].cmd_name, cmd_data_arr[i].cmd);
	}
	print_bt(cmd_tree);
	
	//Initialising Varriables
	var *var_e = add_var("e");
	var_e->addr=(node *)malloc(sizeof(struct node));
	var_e->addr->node_type=cn_num;
	var_e->addr->method=&m_cnn;
	var_e->addr->d.value=2.718282;
	//Initialising Nodes
	fnc=(node *) malloc(sizeof (node));
	//Initialising Command Queue
	cmd_0=(cmd_q *)malloc(sizeof(cmd_q));
	cmd_0->prev=0;
	cmd_0->next=0;
	return 0;
}
int module_generate (int param_c,char *params[])
{
	FILE *err_log_file=fopen("error_log.txt","a");

	dup2(fileno(err_log_file),STDERR_FILENO);
	start(param_c,params);
return 1;
}
