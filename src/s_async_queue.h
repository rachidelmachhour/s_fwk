
#include "s_queue.h"

#ifndef S_ASYNC_QUEUE_H
#define S_ASYNC_QUEUE_H

struct s_async_queue {
	struct s_queue *queue_d;
	int nb_byte;
	int nb_packet;
	int time;
};

typedef enum {PACKET, BYTE, TIME} queue_mode;

struct s_async_queue async_t;

async_t * new_async_queue(queue_mode mode, int value);

int s_async_queue_try_push(async_t * async_d, void * item);

int s_async_queue_push(async_t * async_d, void * item);

int s_async_queue_try_pull(async_t * async_d, void * item);

int s_async_queue_pull(async_t * async_d, void * item);

int s_async_queue_get_byte_level(async_t * async_d);

int s_async_queue_get_packet_level(async_t * async_d);

int s_async_queue_get_time_level(async_t * async_d);

#endif