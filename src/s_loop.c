/**
 * \file s_loop.c
 * \brief Definition of the sloop functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 29 septembre 2014
 *
 * This is file is used to code the functions needed to use this loop.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#include "s_loop.h"

void sloop_destroy_table(struct sloop_table *table);

void sloop_table_set_fd(struct sloop_table *table,fd_set *fds);

void sloop_table_call_handler(struct sloop_table *table, fd_set *fds);

void * realloc_array(void *ptr, size_t nmemb, size_t size)
{
	if (size && nmemb > (~(size_t) 0) / size)
		return NULL;
	return realloc(ptr, nmemb * size);
}

sloop_t * sloop_new()
{

	sloop_t  *sloop_d = malloc(sizeof(sloop_t) );
	if(sloop_d){
		memset(sloop_d, 0, sizeof(sloop_t));
		sloop_d->finished=0;
	}

	return sloop_d;
}

int sloop_timer_new(sloop_t *sloop_d,int msec, sloop_callback_timer handler, void *user_data )
{
	int id=0;
   id=sloop_add_timer(sloop_d,msec,handler,user_data);
   return id;
}


int sloop_set_timeout(sloop_t *sloop_d, long msec)
{
    sloop_d->timeout.tv_usec=msec*1000;
	return 0;
}

/* sloop_mode must be SLOOP_RD or SLOOP_WR or SLOOP_EXCEPT */

int sloop_add_fd(sloop_t *sloop_d, sloop_mode mode,int fd,sloop_callback_handler handler,void *user_data)
{
	struct sloop *tmp;
	struct sloop_table *table;
	int new_max_fd;

	if (fd > sloop_d->max_fd)
		new_max_fd = fd;
	else
		new_max_fd = sloop_d->max_fd;
    switch (mode) {
    	case SLOOP_RD :
    		table=&sloop_d->readfds;
    		break;
    	case SLOOP_WR :
    		table=&sloop_d->writefds;
    	    break;
    	case SLOOP_EXCEPT :
    		table=&sloop_d->exceptfds;
    		break;
    	default :
    	    table=NULL;
    }

	if (table == NULL)
		return -1;

	tmp = realloc_array(table->table, table->num_fd + 1, sizeof(struct sloop));

	if (tmp == NULL)
		return -1;

	tmp[table->num_fd].fd = fd;
	tmp[table->num_fd].user_data = user_data;
	tmp[table->num_fd].handler = handler;

	table->num_fd++;
	table->table = tmp;
	sloop_d->max_fd = new_max_fd;
	table->changed = 1;

	return 0;
}

int sloop_add_timer(sloop_t *sloop_d, int msec, sloop_callback_timer handler, void *user_data)
{
	struct sloop_timer *tmp;

	if(msec == 0)
		return -1;

	tmp = realloc_array(sloop_d->timer_d.timer, sloop_d->timer_d.num_timer+1 , sizeof(struct sloop_timer));

    tmp[sloop_d->timer_d.num_timer].time_w=msec;
    tmp[sloop_d->timer_d.num_timer].handler=handler;
    tmp[sloop_d->timer_d.num_timer].user_data=user_data;
    tmp[sloop_d->timer_d.num_timer].id=sloop_d->timer_d.num_timer;
    sloop_d->timer_d.timer=tmp;
	sloop_d->timer_d.num_timer++;
	return sloop_d->timer_d.num_timer-1;
}


//sloop_table must be from sloop_data.readfds or sloop_data.writefds or sloop_data.exceptfds 

int sloop_remove_fd(sloop_t *sloop_d,sloop_mode mode, int fd)
{
	int i;
	struct sloop_table *table;

	switch (mode) {
    	case SLOOP_RD :
    		table=&sloop_d->readfds;
    		break;
    	case SLOOP_WR :
    		table=&sloop_d->writefds;
    	    break;
    	case SLOOP_EXCEPT :
    		table=&sloop_d->exceptfds;
    		break;
    	default :
    	    table=NULL;
    }

	if (table == NULL || table->table == NULL || table->num_fd == 0)
		return -1;

	for (i = 0; i < table->num_fd; i++) {
		if (table->table[i].fd == fd)
			break;
	}
	if (i == table->num_fd)
		return -1;

	if (i != table->num_fd - 1) {
		memmove(&table->table[i], &table->table[i + 1],(table->num_fd - i - 1) * sizeof(struct sloop));
	}
	table->num_fd--;
	table->changed = 1;
	return 0;
}

int sloop_remove_timer(sloop_t *sloop_d, int id)
{
	int i;
	if (sloop_d->timer_d.timer == NULL || sloop_d->timer_d.num_timer == 0)
		return -1;

	for(i=0;i<sloop_d->timer_d.num_timer;i++)
	{
		if(id == sloop_d->timer_d.timer[i].id){
			if (id != sloop_d->timer_d.num_timer - 1) {
				memmove(&sloop_d->timer_d.timer[id], &sloop_d->timer_d.timer[id+1],(sloop_d->timer_d.num_timer - id - 1) * sizeof(struct sloop_timer));
			}
		}
	}	
	sloop_d->timer_d.num_timer--;
	return 0;
}

int sloop_stop_timer(sloop_timer_t *timer_d)
{
	if(timer_d==NULL)
		return -1;

	timer_d->timer->stop=1;
	return 0;
}

void sloop_read_fd(int fd)
{
	fd_set readfds;

	if (fd < 0)
		return;

	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	select(fd + 1, &readfds, NULL, NULL, NULL);
}

void sloop_destroy_table(struct sloop_table *table)
{
	if (table) 	
		free(table->table);
}

void sloop_table_set_fd(struct sloop_table *table,fd_set *fds)
{
	int i;

	FD_ZERO(fds);

	if (table->table == NULL)
		return;

	for (i = 0; i < table->num_fd; i++) {
		FD_SET(table->table[i].fd, fds);
	}
}

void sloop_table_call_handler(struct sloop_table *table, fd_set *fds)
{
	int i;

	if (table == NULL || table->table == NULL)
		return;

	table->changed = 0;
	for (i = 0; i < table->num_fd; i++) {
		if (FD_ISSET(table->table[i].fd, fds)) {
			table->table[i].handler(table->table[i].fd,table->table[i].user_data);
			if (table->changed)
				break;
		}
	}
}

void sloop_run_step(sloop_t *sloop_d)
{
	fd_set *readfds, *writefds, *exceptfds;
	struct timeval timeout;
	int res;

	readfds = malloc(sizeof(*readfds));
	writefds = malloc(sizeof(*writefds));
	exceptfds = malloc(sizeof(*exceptfds));
	if (readfds == NULL || writefds == NULL || exceptfds == NULL)
	{
		free(readfds);
		free(writefds);
		free(exceptfds);
		return;
	}
	timeout.tv_sec=0;
	timeout.tv_usec=100000;
	timeout.tv_usec=sloop_d->timeout.tv_usec;
    
    sloop_table_set_fd(&sloop_d->readfds, readfds);
	sloop_table_set_fd(&sloop_d->writefds, writefds);
	sloop_table_set_fd(&sloop_d->exceptfds, exceptfds);

	res = select(sloop_d->max_fd + 1, readfds, writefds, exceptfds, &timeout);

	if(res != 0)
	{
		sloop_table_call_handler(&sloop_d->readfds, readfds);
		sloop_table_call_handler(&sloop_d->writefds, writefds);
		sloop_table_call_handler(&sloop_d->exceptfds, exceptfds);
	}
		/* check timer */

	else
	{
		printf("timeout finished !! no event !!");
	}

	free(readfds);
	free(writefds);
	free(exceptfds);

	return;
}


void sloop_run(sloop_t *sloop_d)
{
	fd_set *readfds, *writefds, *exceptfds;
	struct timeval timeout;
	int res,i;

	readfds = malloc(sizeof(*readfds));
	writefds = malloc(sizeof(*writefds));
	exceptfds = malloc(sizeof(*exceptfds));
	if (readfds == NULL || writefds == NULL || exceptfds == NULL)
	{
		free(readfds);
		free(writefds);
		free(exceptfds);
		return;
	}
	timeout.tv_sec=0;
	timeout.tv_usec=500000;
    
	while(!sloop_d->finished)
	{

	sloop_table_set_fd(&sloop_d->readfds, readfds);
	sloop_table_set_fd(&sloop_d->writefds, writefds);
	sloop_table_set_fd(&sloop_d->exceptfds, exceptfds);

	res = select(sloop_d->max_fd + 1, readfds, writefds, exceptfds, &timeout);

	if(res != 0)
	{
		sloop_table_call_handler(&sloop_d->readfds, readfds);
		sloop_table_call_handler(&sloop_d->writefds, writefds);
		sloop_table_call_handler(&sloop_d->exceptfds, exceptfds);
	}	/* check timer */
    
    if(sloop_d->timer_d.num_timer!=0){

    	double actual_time[sloop_d->timer_d.num_timer],first_time[sloop_d->timer_d.num_timer];

		for(i=0;i<sloop_d->timer_d.num_timer;i++)
			{
    			first_time[i]=(double)time(NULL);
			}

		for(i=0;i<sloop_d->timer_d.num_timer;i++)
		{
						actual_time[i]=(double)time(NULL);
			if(sloop_d->timer_d.timer[i].time_w<(actual_time[i]-first_time[i]))
			{
				sloop_d->timer_d.timer[i].handler(sloop_d->timer_d.timer[i].user_data);
				first_time[i]=(double)time(NULL);
			}
		}
	}

	/*else
	{
		printf("timeout finished !! no event !!");
	}*/
	}	
	free(readfds);
	free(writefds);
	free(exceptfds);

	return;
}

size_t  sloop_read_buffer(int fd, void *buf, size_t count)
{
	size_t nread;
	nread = read(fd,buf,count);
	return nread;
}

int sloop_get_fd(sloop_t *sloop_d,sloop_mode mode,int id)
{
	struct sloop_table *table;

	switch (mode) {
    	case SLOOP_RD :
    		table=&sloop_d->readfds;
    		break;
    	case SLOOP_WR :
    		table=&sloop_d->writefds;
    	    break;
    	case SLOOP_EXCEPT :
    		table=&sloop_d->exceptfds;
    		break;
    	default :
    	    table=NULL;
    }
    if (table == NULL)
		return -1;

	return table->table[id].fd;
}

void sloop_destroy(sloop_t *sloop_d)
{
	sloop_d->finished=1;
	sloop_destroy_table(&sloop_d->readfds);
	sloop_destroy_table(&sloop_d->writefds);
	sloop_destroy_table(&sloop_d->exceptfds);
}
