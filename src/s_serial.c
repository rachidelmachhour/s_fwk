

#include "s_serial.h"



unsigned char comports[30][16]={"/dev/ttyAMA0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4","/dev/ttyS5",
                       "/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9","/dev/ttyS10","/dev/ttyS11",
                       "/dev/ttyS12","/dev/ttyS13","/dev/ttyS14","/dev/ttyS15","/dev/ttyUSB0",
                       "/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4","/dev/ttyUSB5",
                       "/dev/ttyAMA0","/dev/ttyAMA1","/dev/ttyACM0","/dev/ttyACM1",
                       "/dev/rfcomm0","/dev/rfcomm1","/dev/ircomm0","/dev/ircomm1"};



                       
s_serial_t  *s_serial_new()
{
	s_serial_t * s_serial;
	s_serial_m * mode;
	s_serial= (s_serial_t *)malloc(sizeof(s_serial_t));
	mode= (s_serial_m *)malloc(sizeof(s_serial_m));
	s_serial->mode=mode;
	return s_serial;
}

int s_serial_set_comport(s_serial_t * s_serial,unsigned char *comport)
{
	for (int i = 0; i <sizeof (comports) / sizeof (comports[0]) ; ++i)
	{
		if(strcmp (comport, comports[i])==0)
		{
			s_serial->comport=comport;
			return 1;
		}
	}
    return 0;
}



int s_serial_set_mode(s_serial_t * s_serial,unsigned char * mode)
{
  if(strlen(mode) != 3)
  {
    printf("invalid mode \"%s\"\n", mode);
    return(1);
  }

  switch(mode[0])
  {
    case '8': s_serial->mode->cbits = CS8;
              break;
    case '7': s_serial->mode->cbits = CS7;
              break;
    case '6': s_serial->mode->cbits = CS6;
              break;
    case '5': s_serial->mode->cbits = CS5;
              break;
    default : printf("invalid number of data-bits '%c'\n", mode[0]);
              return(0);
              break;
  }

  switch(mode[1])
  {
    case 'N':
    case 'n': s_serial->mode->cpar = IGNPAR;
              break;
    case 'E':
    case 'e': s_serial->mode->cpar = PARENB;
              break;
    case 'O':
    case 'o': s_serial->mode->cpar = (PARENB | PARODD);
              break;
    default : printf("invalid parity '%c'\n", mode[1]);
              return(0);
              break;
  }

  switch(mode[2])
  {
    case '1': s_serial->mode->bstop = 0;
              break;
    case '2': s_serial->mode->bstop = CSTOPB;
              break;
    default : printf("invalid number of stop bits '%c'\n", mode[2]);
              return(0);
              break;
  }

  return 1;

}
int s_serial_set_nb_stop_bits(s_serial_t * s_serial,int nb_stop_bits)
{
	switch(nb_stop_bits)
   	{
	    case 1  :   s_serial->settings.c_cflag &= ~CSTOPB; // Not 2 stop bits = One stop bit
	    			s_serial->mode->bstop=0;
				    break;
		case 2  :	s_serial->settings.c_cflag |= CSTOPB; // Two stop bits
			    	s_serial->mode->cpar=CSTOPB;
			        break;
	    default :   printf("invalid number of stop bits '%d'\n", nb_stop_bits);
	                return(0);
	                break;
    }
    tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings);


  return 1;
}

int	s_serial_set_parity_type(s_serial_t * s_serial,unsigned char parity_type)
{
	switch(parity_type)
  	{
	    case 'N':
	    case 'n':	s_serial->settings.c_cflag &= ~PARENB;
	    		 	s_serial->mode->cpar = IGNPAR;	    		 	
	             	break;
	    case 'E':		    
	    case 'e':	s_serial->settings.c_cflag |= PARENB; // Parity enable
					s_serial->settings.c_cflag &= ~PARODD; // Turn off odd parity = even
	    			s_serial->mode->cpar = PARENB;
	             	break;
	    case 'O':		    
	    case 'o':	s_serial->settings.c_cflag = (PARENB | PARODD);
	    			s_serial->mode->cpar = (PARENB | PARODD);
	             	break;
	    default :	printf("invalid parity '%c'\n", parity_type);
	             	return(0);
	             	break;
  	}
  	tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings);

  return 1;
}

int	s_serial_set_caractere_size(s_serial_t * s_serial,int unsigned char_size)
{
	switch(unsigned char_size)
	{
	    case '8': s_serial->settings.c_cflag &= ~CSIZE;
	    		  s_serial->settings.c_cflag |= CS8; 
	    		  s_serial->mode->cbits = CS8;
                  break;
	    case '7': s_serial->settings.c_cflag &= ~CSIZE;
	    		  s_serial->settings.c_cflag |= CS7;
	    		  s_serial->mode->cbits = CS7;
                  break;
	    case '6': s_serial->settings.c_cflag &= ~CSIZE;
	    		  s_serial->settings.c_cflag |= CS6;
	    		  s_serial->mode->cbits = CS6;
                  break;
	    case '5': s_serial->settings.c_cflag &= ~CSIZE;
	    		  s_serial->settings.c_cflag |= CS5;
	    		  s_serial->mode->cbits = CS5;
				  break;
	    default : printf("invalid number of data-bits '%d'\n", unsigned char_size);
	              return(0);
	              break;
 	}
 	tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings);

  return 1;
}


int s_serial_set_baudr(s_serial_t * s_serial,int  baudrate)
{
	switch(baudrate)
		  {
		    case      50 :  s_serial->baudr = B50;
		                    
							break;
		    case      75 :  s_serial->baudr = B75;
		                     
							break;
		    case     110 :  s_serial->baudr = B110;
		                     
							break;
		    case     134 :  s_serial->baudr = B134;
		                     
							break;
		    case     150 :  s_serial->baudr = B150;
		                     
							break;
		    case     200 :  s_serial->baudr = B200;
		                     
							break;
		    case     300 :  s_serial->baudr = B300;
		                     
							break;
		    case     600 :  s_serial->baudr = B600;
		                     
							break;
		    case    1200 :  s_serial->baudr = B1200;
		                     
							break;
		    case    1800 :  s_serial->baudr = B1800;
		                     
							break;
		    case    2400 :  s_serial->baudr = B2400;
		                     
							break;
		    case    4800 :  s_serial->baudr = B4800;
		                     
							break;
		    case    9600 :  s_serial->baudr = B9600;
		                     
							break;
		    case   19200 :  s_serial->baudr = B19200;
		                     
							break;
		    case   38400 :  s_serial->baudr = B38400;
		                     
							break;
		    case   57600 :  s_serial->baudr = B57600;
		                     
							break;
		    case  115200 :  s_serial->baudr = B115200;
		                     
							break;
		    case  230400 :  s_serial->baudr = B230400;
		                     
							break;
		    case  460800 :  s_serial->baudr = B460800;
		                     
							break;
		    case  500000 :  s_serial->baudr = B500000;
		                     
							break;
		    case  576000 :  s_serial->baudr = B576000;
		                     
							break;
		    case  921600 :  s_serial->baudr = B921600;
		                     
							break;
		    case 1000000 :  s_serial->baudr = B1000000;
		                     
							break;
		    case 1152000 :  s_serial->baudr = B1152000;
		                     
							break;
		    case 1500000 :  s_serial->baudr = B1500000;
		                     
							break;
		    case 2000000 :  s_serial->baudr = B2000000;

							break;
		    case 2500000 :  s_serial->baudr = B2500000;

							break;
		    case 3000000 :  s_serial->baudr = B3000000;

							break;
		    case 3500000 :  s_serial->baudr = B3500000;

							break;
		    case 4000000 :  s_serial->baudr = B4000000;
							break;
		    default      :  printf("invalid baudrate\n");
		                    return(0);
							break;
		  }

  	return 1;
}


int s_serial_set_baudrate(s_serial_t * s_serial,int  baudrate)
{
	
	s_serial_set_baudr(s_serial,baudrate);

	cfsetispeed(&s_serial->settings, s_serial->baudr);

	cfsetospeed(&s_serial->settings, s_serial->baudr);
	if(tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)==-1)
 	{
 	  close(s_serial->fd);
 	  perror("unable to adjust portsettings ");
 	  return(0);
 	}
 return 1;
}

int s_serial_set_all(s_serial_t * s_serial,unsigned char *comport,int baudr,unsigned char* mode)
{
	if(s_serial_set_comport(s_serial,comport)==0)
	{
		printf("illegal comport number\n");
		return 0;
	}
	if(s_serial_set_baudr(s_serial,baudr)==0)
	{
		printf("invalid baudrate \n");
		return 0;
	}
	if(s_serial_set_mode(s_serial,mode)==0)
	{
		printf("invalid mode \n");
		return 0;
	}
	return 1;
}

int	s_serial_set_settings(s_serial_t* s_serial)
{

	memset(&s_serial->settings, 0, sizeof(s_serial->settings)); 
	s_serial->settings.c_cflag = s_serial->mode->cbits | s_serial->mode->cpar | s_serial->mode->bstop | CLOCAL | CREAD;
 	s_serial->settings.c_iflag = IGNPAR;
 	s_serial->settings.c_oflag = 0;
 	s_serial->settings.c_lflag = 0;
 	s_serial->settings.c_cc[VMIN] = 0;      
 	s_serial->settings.c_cc[VTIME] = 0;     
	cfsetispeed(&s_serial->settings, s_serial->baudr);
 	cfsetospeed(&s_serial->settings, s_serial->baudr);
 	if(tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)==-1)
 	{
 	  close(s_serial->fd);
 	  perror("unable to adjust portsettings ");
 	  return(0);
 	}
	 	if(ioctl(s_serial->fd, TIOCMGET, &s_serial->status) == -1)
 	{
 	  perror("unable to get portstatus");
 	  return(0);
 	}
	s_serial->status |= TIOCM_DTR;    /* turn on DTR */
 	s_serial->status |= TIOCM_RTS;    /* turn on RTS */
	 	if(ioctl(s_serial->fd, TIOCMSET, &s_serial->status) == -1)
 	{
 	  perror("unable to set portstatus");
 	  return(0);
  	}
  return 1;
}
void s_serial_enable_hardware_flow_control(s_serial_t *s_serial)
{
	int rc ;

	s_serial->settings.c_cflag |= CRTSCTS;

	if((rc = tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)) < 0){
        fprintf(stderr, "failed to set attr: %d, %s\n", s_serial->fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
}
void s_serial_disable_hardware_flow_control(s_serial_t *s_serial)
{
	int rc ;

	s_serial->settings.c_cflag &= ~CRTSCTS;

	if((rc = tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)) < 0){
        fprintf(stderr, "failed to set attr: %d, %s\n", s_serial->fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
}
void s_serial_enable_software_flow_control(s_serial_t *s_serial)
{
	int rc;

	s_serial->settings.c_iflag |= (IXON | IXOFF | IXANY);

	if((rc = tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)) < 0){
        fprintf(stderr, "failed to set attr: %d, %s\n", s_serial->fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
}


void s_serial_disable_software_flow_control(s_serial_t *s_serial)
{
	int rc ;

	s_serial->settings.c_iflag &= ~(IXON | IXOFF | IXANY);

	if((rc = tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)) < 0){
        fprintf(stderr, "failed to set attr: %d, %s\n", s_serial->fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
	
}



int s_serial_get_fd(s_serial_t * s_serial)
{
	return(s_serial->fd);
}





int s_serial_open(s_serial_t * s_serial,unsigned char *comport,int baudr,unsigned char* mode)
{
	s_serial_set_all(s_serial,comport,baudr,mode);
	 
	s_serial->fd = open(s_serial->comport, O_RDWR | O_NOCTTY | O_NDELAY);
  	if(s_serial->fd==-1)
  	{
    perror("unable to open comport ");
    return(0);
  	}

 	s_serial_set_settings(s_serial);

  return 1;
}




void s_serial_close(s_serial_t * s_serial)
{
	close(s_serial->fd);
}




int s_serial_write_byte(s_serial_t * s_serial, unsigned unsigned char byte)
{
  int n;
  n = write(s_serial->fd, &byte, 1);
  if(n<0)  return(1);

  return(0);
}




int s_serial_write(s_serial_t *s_serial, const unsigned char *text)  /* sends a string to serial port */
{
	int nb=0;
  while(*text != 0)   nb+=s_serial_write_byte(s_serial, *(text++));
  return nb;
}




int s_serial_read(s_serial_t * s_serial, unsigned unsigned char *buf, int size)
{
   	int n;


  		 n = read(s_serial->fd, buf, size);

  return(n);
}



































// int main() 
// {
// 	s_serial_t *s;
// 	int baudr=115200,n,i;
// 	unsigned unsigned char buf[4096];
// 	unsigned char * comport="/dev/ttyUSB0";
// 	unsigned char mode[]={'8','N','1',0};
// 	unsigned char  str[2][512];

//   	strcpy(str[0], "The quick brown fox jumped over the lazy grey dog.\n");

//   	strcpy(str[1], "Happy serial programming!.\n");



// 	s=s_serial_new();

// 	s_serial_open(s,comport,baudr,mode);
// 	//s_serial_read(s,)
// 	  while(1)
// 	{
// 	    n = s_serial_read(s, buf, 4095);

//     if(n > 0)
//     {
//       buf[n] = 0;   /* always put a "null" at the end of a string! */

//       for(i=0; i < n; i++)
//       {
//         if(buf[i] < 32)  /* replace unreadable control-codes by dots */
//         {
//           buf[i] = '.';
//         }
//       }

//       printf("received %i bytes: %s\n", n, (unsigned char *)buf);
//     }

// 		#ifdef _WIN32
//     Sleep(100);
// 		#else
//     usleep(100000);  /* sleep for 100 milliSeconds */
// 		#endif
// 	  }


// 	printf("%d\n",s->baudr );
//     return 0;
// }
