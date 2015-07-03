/**
 * \file s_serial.h
 * \brief Header for the s_serial functions
 * \author Rachid EL Machhour ( SpartSystems )
 * \version 0.1
 * \date 16 decembre 2014
 *
 * This is a header file for the sserial functions, it contains all the prototypes of functions needed to develop an application using this libserial.
 *
 */
#ifndef s_serial_INCLUDED
#define s_serial_INCLUDED


#include <string.h>
#include <stdio.h>




#ifdef __linux__

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>   /* Error number definitions */

#else

#include <windows.h>
#include <stdbool.h>
#endif


/**
 * \struct s_serial_mode
 * \brief define a mode serial.
 *
 *
 */

struct s_serial_mode
{
	int cbits;
    int cpar;
    int bstop;
};

typedef struct s_serial_mode s_serial_m;

/**
 * \struct s_serial_data
 * \brief define all data for serial port.
 *
 *
 */
struct s_serial_data
{
	
	#ifdef _WIN32
    	HANDLE *fd;
	DCB settings;
    	char mode[500];
    	COMMTIMEOUTS Cptimeouts;
	#else
    	struct termios settings;
   	int fd;
   	s_serial_m * mode;
	#endif
	char *comport;
	int baudr;
	int status;
};

typedef struct s_serial_data s_serial_t;



/**
 * \fn s_serial_t *s_serial_new();
 * \brief function used to initialize the s_serial_data.
 *
 * \param NONE
 * \return struct s_serial_data.
 */
s_serial_t *  	s_serial_new();



/**
 * \fn int s_serial_set_settings_default(s_serial_t* s_serial);
 * \brief function used to set the default settings of the s_serial_data.
 *
 * \param s_serial s_serial_t.
 * \return 1 if sucess or 0 if fail.
 */
int 			s_serial_set_settings_default(s_serial_t* s_serial);




/**
 * \fn int s_serial_set_nb_stop_bits(s_serial_t* s_serial,int nb_stop_bits);
 * \brief function used to set the default settings of the s_serial_data.
 *
 * \param s_serial s_serial_t.
 * \return 1 if sucess or 0 if fail.
 */
int 			s_serial_set_nb_stop_bits(s_serial_t * s_serial,int nb_stop_bits);





int 			s_serial_set_parity_type(s_serial_t * s_serial,char parity_type);
int 			s_serial_set_caractere_size(s_serial_t * s_serial,int char_size);
int 			s_serial_open(s_serial_t * s_serial,char *comport);
#ifdef _WIN32
HANDLE 			s_serial_get_fd(s_serial_t * s_serial);
#else
int 			s_serial_get_fd(s_serial_t * s_serial);
#endif
int 			s_serial_read(s_serial_t * s_serial,   char *buf, int size);
int 			s_serial_write_byte(s_serial_t * s_serial,   char byte);
int 			s_serial_set_baudrate(s_serial_t * s_serial,int  baudrate);
int 			s_serial_write_text(s_serial_t *s_serial,   char *text);  /* sends a string to serial port */
int 			s_serial_write(s_serial_t * s_serial, unsigned char * buffer, int len);
void			s_serial_close(s_serial_t * s_serial);
void 			s_serial_enable_hardware_flow_control(s_serial_t *s_serial);
void 			s_serial_enable_software_flow_control(s_serial_t *s_serial);
void 			s_serial_disable_hardware_flow_control(s_serial_t *s_serial);
void 			s_serial_disable_software_flow_control(s_serial_t *s_serial);






#endif
