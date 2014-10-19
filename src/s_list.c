/**
 * \file slist.c
 * \brief Definition of the slist functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 29 septembre 2014
 *
 * This is file is used to code the functions needed to use this list.
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include "s_list.h"

struct s_list *s_list_new()
{
	struct s_list *list;
	list = (struct s_list *)malloc(sizeof(struct s_list));
	list->next=NULL;
	list->data=NULL;
	list->length=0;
	return list;
}

int s_list_insert_first(struct s_list *list_d, void *item)
{
	struct s_list *list;

	if(list_d==NULL)
		return -1;

	list = (struct s_list *)malloc(sizeof(struct s_list));
	list->next=list_d->next;
	list->data=item;
	list_d->next=list;
	list_d->length=list_d->length+1;

	return 0;
}

int s_list_clean(struct s_list *list_d)
{
	list_d->next=NULL;
	list_d->length=0;
}

int s_list_destroy(struct s_list *list_d)
{
	if(list_d == NULL)
		return -1;
	
	free(list_d);
}

int s_list_is_empty(struct s_list *list_d)
{
	if(list_d->next==NULL)
		return 1;
	else
		return 0;
}

int s_list_get_length(struct s_list *list_d)
{
	return list_d->length;
}

int s_list_get_position(struct s_list *list_d, void *item)
{
	int i,len=0;
	struct s_list *list;
	list=list_d->next;
	len = s_list_get_length(list_d);

	for(i=0;i<len;i++)
	{
		if(list->data==item)
			return i;
		list=list->next;
	}
	return -1;
}

int s_list_insert_at_position(struct s_list* list_d, int position,void *data)
{
	int i;
	struct s_list *list,*tmp;
	tmp = (struct s_list *)malloc(sizeof(struct s_list));
    if(position>=list_d->length)
    	return -1;
	list=list_d->next;
	for(i=0;i<position-1;i++)
			list=list->next;
	tmp->next=list->next;
	list->next=tmp;
	tmp->data=data;
	list_d->length++;
	return 0;

}

int s_list_print(struct s_list *list_d,slist_callback_print handler)
{
	int i,len;
	struct s_list *list;
	list=list_d->next;
	len = s_list_get_length(list_d);
	if(len==0)
	{
		return -1;
	}
	for(i=0;i<len;i++)
	{
		if(list==NULL)
		{
		return 0;
		}			
		handler(list->data);
		list=list->next;
	}
}

int s_list_insert_last(struct s_list *list_d, void *item)
{
	struct s_list *list,*tmp;
	int i,len;
	list=list_d->next;

	if(list_d==NULL)
		return -1;

	if(list == NULL)
	{
		tmp = (struct s_list *)malloc(sizeof(struct s_list));
		tmp->data = item;
		tmp->next=NULL;
		list_d->next=tmp;
		list_d->length++;
		return 0;
	}
	len = s_list_get_length(list_d);
	if(len==1)
	{
		tmp = (struct s_list *)malloc(sizeof(struct s_list));
		tmp->data = item;
		list->next=tmp;
		tmp->next=NULL;
		list_d->length++;
	}
	else
	{
		for(i=0;i<len-1;i++)
			list=list->next;
		tmp = (struct s_list *)malloc(sizeof(struct s_list));
		tmp->data = item;
		list->next=tmp;
		tmp->next=NULL;
		list_d->length++;
	}
	return 0;

}

int s_list_delete_item(struct s_list *list_d, void *item)
{
	struct s_list *list,*tmp;
	int i,pos;
	pos = s_list_get_position(list_d,item);
    if(pos == -1)
    {
		return -1;
    }
	list=list_d->next;	

	for(i=0;i<pos-1;i++)
	{
		list = list->next;
	}
	tmp=list;
	list = list->next;
	tmp->next=list->next;
	list_d->length--;

	return 0;
}

int s_list_delete_at_position(struct s_list *list_d, int position)
{
	struct s_list *list,*tmp;
	int i;
	list=list_d->next;	

	if(position==1)
	{
	tmp=list;
	list_d->next=tmp->next;
	list_d->length--;
	return 0;
	}

	for(i=0;i<position-1;i++)
	{
		list = list->next;
	}
	tmp=list;
	list = list->next;
	tmp->next=list->next;
	list_d->length--;

	return 0;
}

int s_list_delete_last(struct s_list *list_d)
{
	struct s_list *list;
	int i,len;
	list=list_d->next;	
	if(list==NULL)
		return -1;
	len = s_list_get_length(list_d);

	for(i=0;i<len-2;i++)
	{
		list=list->next;
	}
	list->next=NULL;
	list_d->length--;
	return 0;
}

void * s_list_get_data_at_position(struct s_list *list_d,int position)
{
	void *ret;
	int i;	
	struct s_list *list;

    if(position>=list_d->length)
    	return (void *)-1;

	list=list_d->next;
	for(i=0;i<position;i++)
			list=list->next;
	ret=(void *)list->data;
	return ret;
}


