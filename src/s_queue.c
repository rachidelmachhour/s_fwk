/**
 * \file squeue.c
 * \brief Definition of the squeue functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 29 septembre 2014
 *
 * This is file is used to code the functions needed to use this queue.
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include "s_queue.h"

struct s_queue * s_queue_new()
{
	struct s_queue *queue_d;
	queue_d= (struct s_queue *)malloc(sizeof(struct s_queue));
	queue_d->list_d=s_list_new();
	return queue_d;
}

int s_queue_destroy(struct s_queue *queue_d)
{
	if(queue_d == NULL)
		return -1;
	
	free(queue_d);
}

int s_queue_is_empty(struct s_queue *queue_d)
{
	if(queue_d == NULL)
		return -1;

	if(queue_d->list_d->length==0)
		return 1;

	return 0;
}

int s_queue_get_length(struct s_queue *queue_d)
{
	return queue_d->list_d->length;
}

int s_queue_push(struct s_queue *queue_d, void *item)
{
	int res;
	res=s_list_insert_last(queue_d->list_d,item);
	return res;
}

int s_queue_push_first(struct s_queue *queue_d, void *item)
{
	int res;
	res=s_list_insert_first(queue_d->list_d,item);
	return res;
}

int s_queue_push_at_position(struct s_queue *queue_d, void *item, int position)
{
	int res;
	res=s_list_insert_at_position(queue_d->list_d,position,item);
	return res;
}

void * s_queue_pull(struct s_queue *queue_d)
{
	void *ret;
	ret=s_list_get_data_at_position(queue_d->list_d,0);
	s_list_delete_at_position(queue_d->list_d,1);
	return ret;
}

void * s_queue_pull_at_position(struct s_queue *queue_d, int position)
{
	void *ret;
	ret=s_list_get_data_at_position(queue_d->list_d,position);
	s_list_delete_at_position(queue_d->list_d,position);
	return ret;
}

int s_queue_show(struct s_queue *queue_d,squeue_callback_print handler)
{
	int res;
	res=s_list_print(queue_d->list_d,handler);
	return res;
}

