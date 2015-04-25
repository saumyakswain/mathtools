#ifndef MATSOLVE_H
#define MATSOLVE_H

typedef struct fn fn;
typedef struct methods methods;
typedef struct node node;
typedef struct var var;
typedef struct mat mat;
typedef struct cmd_q cmd_q;
typedef struct fn_data fn_data;
typedef struct cmd_data cmd_data;

/******
 * enum
 ******/
enum fnl
{
	op_plus=1,op_minus,op_asterick,op_slash,op_pow,op_der,
	fn_sin=11,fn_cos,fn_tan,fn_asin,fn_acos,fn_atan,fn_sec,fn_cosec,fn_cot,fn_asec,fn_acosec,fn_acot,
	fn_sinh=31,fn_cosh,fn_tanh,fn_asinh,fn_acosh,fn_atanh,fn_sech,fn_cosech,fn_coth,fn_asech,fn_acosech,fn_acoth,
	fn_exp=51,fn_ln,fn_log,
	cn_num=101,ar_mat,
	fn_var=151,
	fn_det=201,
	cmd_evl=301,cmd_slv,cmd_nwt,cmd_RK,cmd_sm,cmd_sm3
};
/***enum****/
/****symbols defined in matsolve.c****/ 
int m_init();
int start(int,char *[]);
int module_generate (int,char *[]);
/****matsolve.c****/ 
/****symbols defined in node.c****/
struct fn
{
	struct node *node1,*node2;
};

struct node
{
	int node_type,extra;
	union
	{
		double value;
		node *node_ptr;
		fn fn_ptr;
		var *var_ptr;
		mat *mat_ptr;
	} d;
	methods *method;
};
struct fn_data
{
	char *fn_name;
	int fn_type;
	methods *method;
};

node *n_err(node *, node *);node *n_def(node *, node *);
node *n_add(node *, node *);node *n_sub(node *, node *);node *n_mul(node *, node *);
node *n_div(node *, node *);node *n_pow(node *, node *);node *n_der(node *, node *);
node *n_var(node *, node *);node *n_cnn(node *, node *);
node *n_mat(node *, node *);

/*
 * Functions returning the value as a double.
 * cannot be used for compound data types.
 */
double d_err(node *);double d_def(node*);
double d_add(node *);double d_sub(node *);double d_mul(node *);double d_div(node *);
double d_pow(node *);double d_cnn(node *);double d_mat(node *);double d_sin(node *);
double d_cos(node *);double d_tan(node *);double d_asin(node *);double d_acos(node *);
double d_atan(node *);double d_sec(node *);double d_cosec(node *);double d_cot(node *);
double d_asec(node *);double d_acosec(node *);double d_acot(node *);double d_sinh(node *);
double d_cosh(node *);double d_tanh(node *);double d_asinh(node *);double d_acosh(node *);
double d_atanh(node *);double d_var(node *);double d_der(node *);double d_ln(node *);
double d_log(node *);double d_exp(node *);

node *n_dr_var(node *,var *,node *);

double d_dr_var(node *, var *);double d_dr_cnn(node *, var *);
double d_dr_add(node *, var *);double d_dr_sub(node *, var *);double d_dr_mul(node *, var *);
double d_dr_pow(node *, var *);
double d_dr_sin(node *, var *);double d_dr_cos(node *, var *);double d_dr_tan(node *, var *);
double d_dr_ln(node *, var *);double d_dr_log(node *, var *);double d_dr_exp(node *, var *);

struct methods
{
	node *(* eval_n)(node *, node *);
	double (* eval_v)(node *);
	node *(* der_n)(node *,var *, node *);
	double(* der_v)(node *, var *);
};


/*****Symbols defined in mat.c****/

struct mat
{
	int r;
	int c;
	double *addr;
};
int print_mat(struct mat *);
struct mat *mat_mul(struct mat *,struct mat *);
struct mat *mat_add(struct mat *,struct mat *);
struct mat *mat_sub(struct mat *,struct mat *);
struct mat *eliminate(struct mat *);
int swap_row(struct mat **,int,int);
double det(struct mat *);
/***mat.c***/


/***symbols defined in common.c***/
double modulus(double);
int get_fn(char **);
int validate_fn(char *, int);
int store_fn(char *, int, struct node *);
int parse_fn(char *,int ,struct node *);
int chk_brace(char *);
int bin_op(char,int,methods *,char *,int,struct node *);
int parse_args(char *,int,char **,int *);
int trunc_str(char **,int *);
int store_mat(char **, struct node *);
int store_num(char *,int ,struct node *);
int parse_ssv(char *,char **);
int match_fnc(char *);
double d(struct var *, char *);
struct node *eval(struct node *);
int arg_err(char *,int ,int, struct node *);
double evaluate(struct node *);
double differentiate(struct node *, struct node *);
int optimise_bin_op_node(node *);
int optimise_fn_node(node *);
int updt_num_node(node *, double);
int plot(node *, var *, double, double, int);
/*****common.c*****/


/*****symbols defined in cmd.c******/
struct cmd_q
{
	struct cmd_q *prev;
	char *cmd_str;
	int cmd_len;
	cmd_q *next;
};
struct cmd_data
{
	char *cmd_name;
	int (*cmd)(char *,int);
};
int exec_cmd(char *, int);
int match_cmd(char *);
int exec_cmd_eval(char *,int);
int exec_cmd_newton(char *,int);
int exec_cmd_RK(char *,int);
int exec_cmd_simpson(char *,int);
int exec_cmd_simpson3(char *,int);
int exec_cmd_mat(char *,int);
int exec_cmd_plot(char *,int);
/****cmd.c***/


/****symbols defined in var.c*****/
struct var
{
	char *name;
	char *var_fn_str;
	int var_fn_len;
	node *addr;
};
struct var *add_var(char *);
/*****var.c*****/


/****symbols defined in num_methods.c****/
double newton(node *,var *,double);
double RK(node *, var *, var *, double, double, double, int);
double simpson3(node *, var *, double, double, int);
double simpson(node *, var *, double, double, int);
/****num_methods.c***/

/****symbols defined in btree.c****/
typedef struct bt_str_node
{
	char *index;
	void *value;
	int height;
	int bal;
	struct bt_str_node *p,*l,*r;
}bt_str_node;
int push_node(bt_str_node **, char *, void *);
void * search_node(bt_str_node *, char *);
void print_bt(bt_str_node *);
void LR();
void LL();
void RL();
void RR();
/****btree.c****/


#endif
