#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matsolve.h"
#include "extern.c"

void update(bt_str_node *r)
{
	int lh=0, rh=0;
	if(r->l) lh = (r->l->height) + 1;
	if(r->r) rh = (r->r->height) + 1;
	r->bal = lh - rh;
	r->height = rh >= lh ? rh : lh;
}
void LL(bt_str_node *root)
{
	bt_str_node tmp,*tmpl;
	tmp = *root;
	*root = *(root->l);
	tmpl=tmp.l;
	tmp.l = root->r;
	root->r = tmpl;
	*(tmpl) = tmp;
	update(root->l);
	update(root->r);
	update(root);
}
void RR(bt_str_node *root)
{
	bt_str_node tmp,*tmpr;
	tmp = *root;
	*root = *(root->r);
	tmpr=tmp.r;
	tmp.r = root->l;
	root->l = tmpr;
	*(tmpr) = tmp;
	update(root->l);
	update(root->r);
	update(root);
}
void LR(bt_str_node *root)
{
	bt_str_node tmp,*tmpr,*tmpl;
	tmp = *root;
	*root = *(root->l->r);
	tmpl=root->l;
	tmpr=root->r;
	root->r = tmp.l->r;
	root->l = tmp.l;
	tmp.l->r=tmpl;
	tmp.l=tmpr;
	*(root->r) = tmp;
	update(root->l);
	update(root->r);
	update(root);
}
void RL(bt_str_node *root)
{
	bt_str_node tmp,*tmpr,*tmpl;
	tmp = *root;
	*root = *(root->r->l);
	tmpl=root->l;
	tmpr=root->r;
	root->r = tmp.r;
	root->l = tmp.r->l;
	tmp.r->l=tmpr;
	tmp.r=tmpl;
	*(root->l) = tmp;
	update(root->l);
	update(root->r);
	update(root);
}
int push_node(bt_str_node **root, char *index, void *value)
{
	if(*root)
	{
		bt_str_node *r=*root;
		int end_node=0, lh=0, rh=0;
		int cmp=strcmp(r->index,index);
		if(!(r->r) && !(r->l))
		{
			end_node=1;
		}
		
		if(cmp < 0)
		{
			push_node(&(r->r), index, value);
		}
		else if(cmp > 0)
		{
			push_node(&(r->l), index, value);
		}
		else if(cmp==0)
		{
			r->value = value;
			return 0;
		}
		if(end_node)
		{
			if(r->l) lh = (r->l->height) + 1;
			if(r->r) rh = (r->r->height) + 1;
			r->bal = lh - rh;
			r->height = 1;
		}
		else
		{
			if(r->l) lh = (r->l->height) + 1;
			if(r->r) rh = (r->r->height) + 1;
			r->bal = lh - rh;
			r->height = rh >= lh ? rh : lh;
			if(r->bal == 2 && r->l->bal == 1) LL(r);
			if(r->bal == 2 && r->l->bal == -1) LR(r);
			if(r->bal == -2 && r->r->bal == 1) RL(r);
			if(r->bal == -2 && r->r->bal == -1) RR(r);
		}

	}
	else
	{
		*root = (bt_str_node *)malloc(sizeof(bt_str_node));
		memset((*root),0,sizeof(bt_str_node));
		(*root)->index=index;
		(*root)->value=value;
		(*root)->height=0;
	}
	return 0;
}

void *search_node(bt_str_node *root, char *index)
{
	int r;
	while(root && (r = strcmp(root->index,index)) != 0)
	{
		root = r < 0 ? root->r : root->l;
	}
	return root && strcmp(root->index,index)==0 ? root->value : 0;
}

void print_bt(bt_str_node *r)
{
	if(r->l) print_bt(r->l);
	if(db)fprintf(stderr, "\nheight:%d, name:%s, bal:%d\n",r->height,r->index,r->bal);
	if(r->r) print_bt(r->r);
}
