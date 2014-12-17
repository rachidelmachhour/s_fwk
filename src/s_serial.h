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




struct s_serial_mode
{
	int cbits;
    int cpar;
    int bstop;
};

typedef struct s_serial_mode s_serial_m;


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




s_serial_t *  	s_serial_new();
int 			s_serial_set_settings_default(s_serial_t* s_serial);
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
int 			s_serial_write(s_serial_t *s_serial,   char *text);  /* sends a string to serial port */
void			s_serial_close(s_serial_t * s_serial);
void 			s_serial_enable_hardware_flow_control(s_serial_t *s_serial);
void 			s_serial_enable_software_flow_control(s_serial_t *s_serial);
void 			s_serial_disable_hardware_flow_control(s_serial_t *s_serial);
void 			s_serial_disable_software_flow_control(s_serial_t *s_serial);






#endif