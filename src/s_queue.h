/**
 * \file s_queue.h
 * \brief Header for the s_queue functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 01 octobre 2014
 *
 * This is a header file for the s_queue functions, it contains all the prototypes of functions needed to develop an application using this queue.
 *
 */

#include "s_list.h"

#ifndef S_QUEUE_H
#define S_QUEUE_H

typedef void (*squeue_callback_print)(void *user_data);

/**
 * \struct s_queue
 * \brief define a queue.
 *
 * this queue is used to define a fifo queue
 * - list_d : the list of the queue. 
 *
 */
struct s_queue {
	struct s_list *list_d;
};

/**
 * \fn struct s_queue * s_queue_new();
 * \brief function used to initialize the s_queue.
 *
 * \param NONE
 * \return struct s_queue.
 */
struct s_queue * s_queue_new();

/**
 * \fn int s_queue_destroy(struct s_queue *queue_d);
 * \brief Destroy the queue.
 *
 * \param queue_d : the queue.
 * \return 0 if success , -1 if fail.
 */
int s_queue_destroy(struct s_queue *queue_d);

/**
 * \fn int s_queue_is_empty(struct s_queue *queue_d);
 * \brief check if the queue is empty.
 *
 * \param queue_d : the queue.
 * \return 1 if empty, 0 if not empty, -1 if error.
 */
int s_queue_is_empty(struct s_queue *queue_d);

/**
 * \fn int s_queue_get_length(struct s_queue *queue_d);
 * \brief get the length of the queue.
 *
 * \param queue_d : the queue.
 * \return the length of the queue.
 */
int s_queue_get_length(struct s_queue *queue_d);

/**
 * \fn int s_queue_push(struct s_queue *queue_d, void *item);
 * \brief Insert an element in the end of the queue.
 *
 * \param queue_d : the queue.
 * \param item : the data of this element. 
 * \return 0 if success , -1 if fail.
 */
int s_queue_push(struct s_queue *queue_d, void *item);

/**
 * \fn int s_queue_push_first(struct s_queue *queue_d, void *item);
 * \brief Insert an element in the start of the queue.
 *
 * \param queue_d : the queue.
 * \param item : the data of this element. 
 * \return 0 if success , -1 if fail.
 */
int s_queue_push_first(struct s_queue *queue_d, void *item);

/**
 * \fn int s_queue_push_at_position(struct s_queue *queue_d, void *item, int position);
 * \brief Insert an element in a defined position in the queue.
 *
 * \param queue_d : the queue.
 * \param item : the data of this element. 
 * \param position : the position where the element will be added. 
 * \return 0 if success , -1 if fail.
 */
int s_queue_push_at_position(struct s_queue *queue_d, void *item, int position);

/**
 * \fn void * s_queue_pull(struct s_queue *queue_d);
 * \brief pull the first element in the queue.
 *
 * \param queue_d : the queue.
 * \return the data pulled.
 */
void * s_queue_pull(struct s_queue *queue_d);

/**
 * \fn void * s_queue_pull_at_position(struct s_queue *queue_d, int position);
 * \brief pull the element in a defined position in the queue.
 *
 * \param queue_d : the queue.
 * \param position : the position where the element will be added. 
 * \return the data pulled.
 */
void * s_queue_pull_at_position(struct s_queue *queue_d, int position);

/**
 * \fn int s_queue_show(struct s_queue *queue_d,squeue_callback_print handler);
 * \brief print the queue using the callback defined by the user.
 *
 * \param queue_d : the queue.
 * \param handler : the function to be called to print the value of the data.
 * \return 0 if success , -1 if fail.
 */
int s_queue_show(struct s_queue *queue_d,squeue_callback_print handler);

#endif /* SQUEUE_H */