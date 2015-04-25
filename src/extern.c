#ifndef EXTERN_C
#define EXTERN_C

extern struct methods m_cnn,m_mat,m_add,
m_sub,m_mul,m_div,
m_pow,m_der,m_sin,
m_cos,m_tan,m_asin,
m_acos,m_atan,m_sec,
m_cosec,m_cot,m_asec,
m_acosec,m_acot,m_sinh,
m_cosh,m_tanh,m_asinh,
m_acosh,m_atanh,m_var,
m_ln,m_log,m_exp;

extern int v,db;
extern node *fnc;
extern var *vars[];
extern int var_pos;
extern bt_str_node *fn_tree, *var_tree, *cmd_tree;

#endif
