
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "s_queue.h"
#include <stdint.h>
#include "s_thread.h"

#ifndef S_ASYNC_QUEUE_H
#define S_ASYNC_QUEUE_H

typedef uint64_t (*async_callback_timer)();

typedef void (*async_callback_print)(void *user_data);

struct s_async_queue {
	struct s_queue *queue_d;
	s_mutex_t mutex;
	s_cond_t  cond_wait_full;
	s_cond_t  cond_wait_empty;
	async_callback_timer time_func;
	int time_custom;
	int nb_byte;
	int byte_limit;
	int nb_packet;
	int packet_limit;
	uint64_t time_us;
	uint64_t time_last;
	uint64_t timestamp[2048];
	uint64_t time_limit;
};


typedef struct s_async_queue async_t;

async_t * new_async_queue();

int s_async_queue_set_time_func(async_t * async_d, async_callback_timer func);

int s_async_queue_set_byte_level(async_t * async_d, int value);

int s_async_queue_set_packet_level(async_t * async_d, int nb);

int s_async_queue_set_time_level(async_t * async_d, uint64_t time_us);

int s_async_queue_try_push(async_t * async_d, void * item);

int s_async_queue_push(async_t * async_d, void * item);

void * s_async_queue_try_pull(async_t * async_d);

void * s_async_queue_pull(async_t * async_d);

int s_async_queue_get_byte_level(async_t * async_d);

int s_async_queue_get_packet_level(async_t * async_d);

uint64_t s_async_queue_get_time_level(async_t * async_d);

void s_async_show(async_t * async_d,async_callback_print handler);

#endif