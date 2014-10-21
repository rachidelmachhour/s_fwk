


#include "s_async_queue.h"


async_t * new_async_queue()
{
	async_t * async_d;
	async_d=(async_t *)malloc(sizeof(async_t));
	async_d->queue_d=s_queue_new();
	async_d->time_custom=0;
	async_d->nb_byte=0;
	async_d->nb_packet=0;
	async_d->time_us=0;
	pthread_mutex_init(&async_d->mutex,NULL);
	pthread_cond_init(&async_d->cond_wait_full,NULL);
	pthread_cond_init(&async_d->cond_wait_empty,NULL);
	return async_d;
}

int s_async_queue_set_time_func(async_t * async_d, async_callback_timer func)
{
	async_d->time_func=func;
	async_d->time_custom=1;
	return 0;
}

int s_async_queue_set_byte_level(async_t * async_d, int value)
{
	async_d->byte_limit=value;
	return 0;
}

int s_async_queue_set_packet_level(async_t * async_d, int nb)
{
	async_d->packet_limit=nb;
	return 0;
}

int s_async_queue_set_time_level(async_t * async_d, uint64_t time_us)
{
	async_d->time_limit=time_us;
	return 0;
}

int s_async_queue_get_byte_level(async_t * async_d)
{
	return async_d->nb_byte;
}

int s_async_queue_get_packet_level(async_t * async_d)
{
	return async_d->nb_packet;
}

uint64_t s_async_queue_get_time_level(async_t * async_d)
{
	return async_d->time_us;
}

int s_async_queue_full(async_t * async_d,int nb_byte,int nb_packet,uint64_t time_us)
{
	int full=0;
	/*if(async_d->time_limit!=0)
	{
		if(async_d->time_us <= async_d->time_limit)
			full=0;
		else
		{
			full=1;
			return full;
		}
	}*/
	if(async_d->packet_limit!=0)
	{
		if(async_d->nb_packet <= async_d->packet_limit)
			full=0;
		else
		{
			full=1;
			return full;
		}
	}
	if(async_d->byte_limit!=0)
	{
		if(async_d->nb_byte <= async_d->byte_limit)
			full=0;
		else
		{
			full=1;
			return full;
		}
	}
	return full;
}

int s_async_queue_try_push(async_t * async_d, void * item)
{
	int ret;
	uint64_t now;
	struct timeval tv;
	int empty=0;
	pthread_mutex_lock(&async_d->mutex);

	empty=s_queue_is_empty(async_d->queue_d);

	if(async_d->time_custom==1)
		now=async_d->time_func();
	else
	{
		if(gettimeofday(&tv,NULL)==-1)
			return -1;
		now = tv.tv_sec*1000000 + tv.tv_usec;
	}

	if(empty==1)
	{
		async_d->time_us=0;
		async_d->time_last=now;
	}
	else
	{
		async_d->time_us=now-async_d->time_last;
		async_d->time_last=now;
	}


	async_d->nb_packet++;
	async_d->nb_byte+=sizeof(item);

	if(s_async_queue_full(async_d,async_d->nb_byte,async_d->nb_packet,async_d->time_us)==0)	
		ret=s_queue_push(async_d->queue_d,item);
	else
		return -1;

	if(empty==1)
	{
		pthread_cond_signal(&async_d->cond_wait_empty);
	}

	pthread_mutex_unlock(&async_d->mutex);
	return ret;
}

int s_async_queue_push(async_t * async_d, void * item)
{
	int ret;
	uint64_t now;
	struct timeval tv;
	int empty=0;
	pthread_mutex_lock(&async_d->mutex);

	empty=s_queue_is_empty(async_d->queue_d);

	if(async_d->time_custom==1)
		now=async_d->time_func();
	else
	{
		if(gettimeofday(&tv,NULL)==-1)
			return -1;
		now = tv.tv_sec*1000000 + tv.tv_usec;
	}

	if(empty==1)
	{
		async_d->time_us=0;
		async_d->time_last=now;
	}
	else
	{
		async_d->time_us=now-async_d->time_last;
		async_d->time_last=now;
	}

	async_d->nb_packet++;
	async_d->nb_byte+=sizeof(item);

	while(s_async_queue_full(async_d,async_d->nb_byte,async_d->nb_packet,now)!=0)
		pthread_cond_wait(&async_d->cond_wait_full,&async_d->mutex);

	ret=s_queue_push(async_d->queue_d,item);

	if(empty==1)
	{
		pthread_cond_signal(&async_d->cond_wait_empty);
	}

	pthread_mutex_unlock(&async_d->mutex);
	return ret;
}


void * s_async_queue_try_pull(async_t * async_d)
{
	void *item;
	int empty=0,full=0;
	pthread_mutex_lock(&async_d->mutex);
	
	if(s_async_queue_full(async_d,async_d->nb_byte,async_d->nb_packet,async_d->time_us)==1)
		full=1;

	empty=s_queue_is_empty(async_d->queue_d);

	if(empty==1)
	{
		printf("async_queue empty !! \n");
		return NULL;
	}

	item=s_queue_pull(async_d->queue_d);

	async_d->nb_packet--;
	async_d->nb_byte-=sizeof(item);

	if(full==1)
		pthread_cond_signal(&async_d->cond_wait_full);

	pthread_mutex_unlock(&async_d->mutex);
	return item;
}


void * s_async_queue_pull(async_t * async_d)
{
	void *item;
	int empty=0,full=0;
	pthread_mutex_lock(&async_d->mutex);
	
	if(s_async_queue_full(async_d,async_d->nb_byte,async_d->nb_packet,async_d->time_us)==1)
		full=1;

	empty=s_queue_is_empty(async_d->queue_d);

	if(empty==1)
	{
		pthread_cond_wait(&async_d->cond_wait_empty,&async_d->mutex);
	}

	item=s_queue_pull(async_d->queue_d);

	async_d->nb_packet--;
	async_d->nb_byte-=sizeof(item);

	if(full==1)
		pthread_cond_signal(&async_d->cond_wait_full);

	pthread_mutex_unlock(&async_d->mutex);
	return item;
}


void s_async_show(async_t * async_d,async_callback_print handler)
{
	s_queue_show(async_d->queue_d, handler);
}



