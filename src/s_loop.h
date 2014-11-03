/**
 * \file s_loop.h
 * \brief Header for the sloop functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 29 septembre 2014
 *
 * This is a header file for the sloop functions, it contains all the prototypes of functions needed to develop an application using this loop.
 *
 */

#ifndef S_LOOP_H
#define S_LOOP_H

#include <math.h>
#ifndef _WIN32
#include <sys/time.h>
#include <sys/select.h>
#else
#include <winsock2.h>
#endif
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include "s_type.h"

typedef void (*sloop_callback_handler)(int fd, void *user_data);

typedef void (*sloop_callback_timer)(void *user_data);

typedef struct sloop_data sloop_t;

typedef struct sloop_table_timer sloop_timer_t;

/**
 * \enum sloop_mode
 * \brief Define the type of sloop mode
 *
 *  Sloop_mode is used to define whether you are using the loop for events comming to mode Read or write or for exceptions.
 *
 */
typedef enum {SLOOP_RD, SLOOP_WR, SLOOP_EXCEPT} sloop_mode;

/**
 * \struct sloop
 * \brief define a simple element of Sloop.
 *
 * Sloop is used to define a simple element os the loop which contains the following informations :
 * - the file descriptor of the element.
 * - the callback handler which is called the this fd receives an event.
 * - the user data which will be included in the callback. 
 *
 */
struct sloop 
{
   int fd;
   void *user_data;
   sloop_callback_handler handler;
};

/**
 * \struct sloop_table
 * \brief define a table of Sloop.
 *
 * the table is used to define a multiple of sloop elements. this table contains the following informations :
 * - num_fd used to define the number of element in the table.
 * - struct sloop *table is the table used to store the elements.
 * - this variable changed is used to see whether the table changed or not. 
 *
 */
struct sloop_table
{
   int num_fd;
   struct sloop *table;
   int changed;
};

/**
 * \struct sloop_timer
 * \brief defines a timer.
 *
 * This timer is used to call a callbach functon in a specific time.
 * - time_w is the time on which the callback is called.
 * - the callback handler which is called the this fd receives an event.
 * - the user data which will be included in the callback.  
 * - the variable stop is used to stop the timer.
 * - the last variable is the id of the timer.
 *
 */
struct sloop_timer
{
   int time_w;
   void *user_data;
   sloop_callback_timer handler;
   int stop;
   int id;
};

/**
 * \struct sloop_table_timer
 * \brief define a table of sloop_timer.
 *
 * the table is used to define a multiple of sloop_timer elements. this table contains the following informations :
 * - num_timer used to define the number of element in the table.
 * - struct sloop_timer *timer is the table used to store the elements.
 *
 */
struct sloop_table_timer
{
   int num_timer;
   struct sloop_timer *timer;
};

/**
 * \struct sloop_data
 * \brief define data of sloop that will be used by the programmer.
 *
 * this data is user by the developper and contains the following informations :
 * - max_fd is a variable which contains the total number of fds.
 * - readfds and writefds and exceptfds are tables which are used in select to know if we are waiting for an event in the mode read , write or exception.
 * - timeout is time that the select wait for an event before it exit.
 * - finished a variable which indicate if the user want to shutdown the select process.
 * - timer_d is the table of timers affected to this sloop_data.  
 *
 */
struct sloop_data
{
	int max_fd;
	struct sloop_table readfds;
	struct sloop_table writefds;
	struct sloop_table exceptfds;
	struct timeval timeout;
   int finished;
   struct sloop_table_timer timer_d;
   int periodic_timer;
};


/**
 * \fn sloop_t *sloop_new();
 * \brief function used to initialize the sloop_data.
 *
 * \param NONE
 * \return struct sloop_data.
 */
sloop_t *sloop_new();

/**
 * \fn int sloop_timer_new(sloop_t *sloop_d,int msec, sloop_callback_timer handler, void *user_data );
 * \brief Function used to create a new timer.
 *
 * \param sloop_d sloop_data.
 * \param msec time in milisecondes
 * \param handler the callback function
 * \param user_data the user data
 * \return 0 if sucess or -1 if fail. 
 */
int sloop_timer_new(sloop_t *sloop_d,int msec, sloop_callback_timer handler, void *user_data );

/**
 * \fn int sloop_add_fd(sloop_t *sloop_d, sloop_mode mode,int fd,sloop_callback_handler handler,void *user_data);
 * \brief function used to add a file descriptor to the sloop_data.
 *
 * \param sloop_d sloop_data.
 * \param mode the mode (SLOOP_RD,SLOOP_WR,SLOOP_EXCEPT).
 * \param fd file descriptor.
 * \param handler the callback function
 * \param user_data the user data
 * \return 0 if sucess or -1 if fail.
 */
int sloop_add_fd(sloop_t *sloop_d, sloop_mode mode,int fd,sloop_callback_handler handler,void *user_data);

/**
 * \fn int sloop_add_timer(sloop_t *sloop_d, int msec, sloop_callback_timer handler, void *user_data);
 * \brief function used to add timer to the sloop_data.
 *
 * \param sloop_d sloop_data.
 * \param msec time in milisecondes
 * \param handler the callback function
 * \param user_data the user data
 * \return 0 if sucess or -1 if fail.
 */
int sloop_add_timer(sloop_t *sloop_d, int msec, sloop_callback_timer handler, void *user_data);

/**
 * \fn int sloop_remove_fd(sloop_t *sloop_d,sloop_mode mode,int fd);
 * \brief function used to remove a file descriptor from the sloop_data.
 *
 * \param sloop_d sloop_data.
 * \param mode the mode (SLOOP_RD,SLOOP_WR,SLOOP_EXCEPT).
 * \param fd file descriptor.
 * \return 0 if sucess or -1 if fail.
 */
int sloop_remove_fd(sloop_t *sloop_d,sloop_mode mode,int fd);

/**
 * \fn int sloop_remove_timer(sloop_t *sloop_d, int id);
 * \brief function used to remove a timer from the sloop_data.
 *
 * \param sloop_d sloop_data.
 * \param id the identification of the timer.
 * \return 0 if sucess or -1 if fail.
 */
int sloop_remove_timer(sloop_t *sloop_d, int id);

/**
 * \fn void sloop_read_fd(int fd);
 * \brief function used to read a single fd and to begin the process of select.
 *
 * \param fd file descriptor
 * \return void
 */
void sloop_read_fd(int fd);

/**
 * \fn void sloop_run_step(sloop_t *sloop_d);
 * \brief function used to begin the loop waiting for an event but only one time.
 *
 * \param sloop_d sloop_data
 * \return void
 */
void sloop_run_step(sloop_t *sloop_d);

/**
 * \fn void sloop_run(sloop_t *sloop_d);
 * \brief function used to begin the loop waiting for an event.
 *
 * \param sloop_d sloop_data
 * \return void
 */
void sloop_run(sloop_t *sloop_d);

/**
 * \fn int sloop_set_timeout(sloop_t *sloop_d , long msec);
 * \brief function used to set the timeout of the sloop_data.
 *
 * \param sloop_d sloop_data.
 * \param msec time in milisecondes
 * \return 0 if sucess or -1 if fail.
 */
int sloop_set_timeout(sloop_t *sloop_d , long msec);

/**
 * \fn void sloop_destroy(sloop_t *sloop_d);
 * \brief function used to destroy the sloop_data.
 *
 * \param sloop_d sloop_data
 * \return void
 */
void sloop_destroy(sloop_t *sloop_d);

#endif /* SLOOP_H */
