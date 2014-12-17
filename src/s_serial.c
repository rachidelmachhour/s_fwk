

#include "s_serial.h"

                       
s_serial_t  *s_serial_new()
{
	s_serial_t * s_serial;
	s_serial_m * mode;
	s_serial= (s_serial_t *)malloc(sizeof(s_serial_t));
	mode= (s_serial_m *)malloc(sizeof(s_serial_m));
	#ifdef __linux__
	s_serial->mode=mode;
	#endif
	return s_serial;
}


int s_serial_set_nb_stop_bits(s_serial_t * s_serial,int nb_stop_bits)
{
	#ifdef _WIN32

	switch(nb_stop_bits)
   	{
	    case 1  :   s_serial->settings.StopBits=ONESTOPBIT;
					break;
		case 1.5:   s_serial->settings.StopBits=ONE5STOPBITS; 
				    break;
		case 2  :	s_serial->settings.StopBits=TWOSTOPBITS;
			        break;
	    default :   printf("invalid number of stop bits '%d'\n", nb_stop_bits);
	    			printf("try : \n 1 : One stop bit. \n 1.5 : 1.5 stop bits. \n 2 : Two stop bits. \n")
	                return(0);
	}
	
	if(SetCommState(s_serial->fd, &s_serial->settings)==0)
	{
		printf("unable to set_nb_stop_bits \n");
		return 0;
	}

	return 1;


	#else /** Linux **/

	switch(nb_stop_bits)
   	{
	    case 1  :   s_serial->settings.c_cflag &= ~CSTOPB; // Not 2 stop bits = One stop bit
	    			s_serial->mode->bstop=0;
				    break;
		case 2  :	s_serial->settings.c_cflag |= CSTOPB; // Two stop bits
			    	s_serial->mode->cpar=CSTOPB;
			        break;
	    default :   printf("invalid number of stop bits '%d'\n", nb_stop_bits);
	    			printf("try : \n 1 : One stop bit \n 2 : Two stop bits \n");
	                return(0);
	}

	if(tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)==-1)
 	{
 	  close(s_serial->fd);
 	  perror("unable to set_nb_stop_bits \n");
 	  return(0);
 	}

    #endif

  return 1;
}

int	s_serial_set_parity_type(s_serial_t * s_serial, char parity_type)
{
	#ifdef _WIN32

	switch(parity_type)
  	{
    	case 'N':
	    case 'n':	s_serial->settings.Parity=NOPARITY; // No parity.
	             	break;
	    case 'E':
	    case 'e':	s_serial->settings.Parity=ODDPARITY; // Odd parity
	             	break;
	    case 'O':
	    case 'o':	s_serial->settings.Parity=SPACEPARITY; // Space parity.
	             	break;
	    default :	printf("invalid parity '%c'\n", parity_type);
	    			printf("try : \n N : No parity \n E :  Parity Enable \n O : Odd Parity \n");
	             	return(0);
	             	break;
  	}
	if(SetCommState(s_serial->fd, &s_serial->settings)==0)
	{
		printf("unable to set_parity_type\n");
	}
	#else

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
	    			printf("try : \n N : No parity \n E :  Parity Enable \n O : Odd Parity \n");
	             	return(0);
  	}

	if(tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)==-1)
 	{
 	  close(s_serial->fd);
 	  perror("unable to set_parity_type ");
 	  return(0);
 	} 
 	#endif

  return 1;
}

int	s_serial_set_caractere_size(s_serial_t * s_serial,int  char_size)
{
	#ifdef _WIN32

	switch(char_size)
	{
	    case  8: s_serial->settings.ByteSize=char_size;
                  break;
	    case  7: s_serial->settings.ByteSize=char_size;
                  break;
	    case  6: s_serial->settings.ByteSize=char_size;
                  break;
	    case  5: s_serial->settings.ByteSize=char_size;
				  break;
	    default : printf("invalid number of data-bits '%d'\n", char_size);
	    		  printf("try : 8,7,6,5 bits\n");
	              return(0);
	              break;
 	}

	if(SetCommState(s_serial->fd, &s_serial->settings)==0)
	{
		printf("unable to set_caractere_size\n");
	}
	#else

	switch(char_size)
	{
	    case  8 : s_serial->settings.c_cflag &= ~CSIZE;
	    		  s_serial->settings.c_cflag |= CS8; 
	    		  s_serial->mode->cbits = CS8;
                  break;
	    case  7 : s_serial->settings.c_cflag &= ~CSIZE;
	    		  s_serial->settings.c_cflag |= CS7;
	    		  s_serial->mode->cbits = CS7;
                  break;
	    case  6 : s_serial->settings.c_cflag &= ~CSIZE;
	    		  s_serial->settings.c_cflag |= CS6;
	    		  s_serial->mode->cbits = CS6;
                  break;
	    case  5 : s_serial->settings.c_cflag &= ~CSIZE;
	    		  s_serial->settings.c_cflag |= CS5;
	    		  s_serial->mode->cbits = CS5;
				  break;
	    default : printf("invalid number of data-bits '%d'\n", char_size);
	    		  printf("try : 8,7,6,5 bits\n");
	              return(0);
	              break;
 	}
	if(tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)==-1)
 	{
 	  close(s_serial->fd);
 	  perror("unable to set_caractere_size \n");
 	  return(0);
 	} 	

 	#endif

  return 1;
}


int s_serial_set_baudrate(s_serial_t * s_serial,int  baudrate)
{

	
	#ifdef _WIN32

	s_serial->settings.BaudRate = baudrate;    
	if(SetCommState(s_serial->fd, &s_serial->settings)==0)
	{
		printf("unable to set_Baudrate\n");
	}

	#else

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

	cfsetispeed(&s_serial->settings, s_serial->baudr);
	cfsetospeed(&s_serial->settings, s_serial->baudr);

	if(tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)==-1)
 	{
 	  close(s_serial->fd);
 	  perror("unable to adjust portsettings ");
 	  return(0);
 	}

 	#endif

 return 1;
}


int	s_serial_set_settings_default(s_serial_t* s_serial)
{
	memset(&s_serial->settings, 0, sizeof(s_serial->settings)); 

	#ifdef _WIN32

	strcpy(s_serial->mode,"baud=115200");
	strcat(s_serial->mode," data=8");
	strcat(s_serial->mode," parity=n");
	strcat(s_serial->mode," stop=1");
	strcat(s_serial->mode," dtr=on rts=on");

	if(!BuildCommDCBA(s_serial->mode, &s_serial->settings))
  	{
	    printf("unable to set comport dcb settings\n");
	    CloseHandle(s_serial->fd);
	    return(0);
  	}

  	if(!SetCommState(s_serial->fd, &s_serial->settings))
  	{
	    printf("unable to set comport cfg settings\n");
	    CloseHandle(s_serial->fd);
	    return(0);
 	}

  	s_serial->Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
  	s_serial->Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  	s_serial->Cptimeouts.ReadTotalTimeoutConstant    = 0;
  	s_serial->Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  	s_serial->Cptimeouts.WriteTotalTimeoutConstant   = 0;

  	if(!SetCommTimeouts(s_serial->fd, &s_serial->Cptimeouts))
  	{
	    printf("unable to set port time-out settings\n");
	    CloseHandle(s_serial->fd);
	    return(0);
  	}	

	#else /** LINUX */
    s_serial->mode->cbits = CS8;
	s_serial->mode->cpar = IGNPAR;
	s_serial->mode->bstop = 0;
	s_serial->baudr=B1152000;
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
	#endif
	
  return 1;
}
void s_serial_enable_hardware_flow_control(s_serial_t *s_serial)
{
	int rc ;
	#ifdef _WIN32
	s_serial->settings.fOutxCtsFlow = true;
    s_serial->settings.fOutxDsrFlow = true;
    s_serial->settings.fDtrControl = DTR_CONTROL_HANDSHAKE;
    s_serial->settings.fOutX = false;
    s_serial->settings.fInX = false;
    s_serial->settings.fRtsControl = RTS_CONTROL_HANDSHAKE;
	#else
	s_serial->settings.c_cflag |= CRTSCTS;

	if((rc = tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)) < 0){
        fprintf(stderr, "failed to set attr: %d, %s\n", s_serial->fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
    #endif
}
void s_serial_disable_hardware_flow_control(s_serial_t *s_serial)
{
	int rc ;
	#ifdef _WIN32
	s_serial->settings.fOutxCtsFlow = false;
    s_serial->settings.fOutxDsrFlow = false;
    s_serial->settings.fDtrControl = DTR_CONTROL_DISABLE;
    s_serial->settings.fOutX = false;
    s_serial->settings.fInX = false;
    s_serial->settings.fRtsControl = RTS_CONTROL_DISABLE;

	#else
	s_serial->settings.c_cflag &= ~CRTSCTS;

	if((rc = tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)) < 0){
        fprintf(stderr, "failed to set attr: %d, %s\n", s_serial->fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
    #endif
}
void s_serial_enable_software_flow_control(s_serial_t *s_serial)
{
	int rc;
	#ifdef _WIN32
	 	s_serial->settings.fOutxCtsFlow = false;
     	s_serial->settings.fOutxDsrFlow = false;
        s_serial->settings.fDtrControl = DTR_CONTROL_DISABLE;
        s_serial->settings.fOutX = true;
        s_serial->settings.fInX = true;
        s_serial->settings.fRtsControl = RTS_CONTROL_DISABLE;
	#else
	s_serial->settings.c_iflag |= (IXON | IXOFF | IXANY);

	if((rc = tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)) < 0){
        fprintf(stderr, "failed to set attr: %d, %s\n", s_serial->fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
    #endif
}


void s_serial_disable_software_flow_control(s_serial_t *s_serial)
{
	int rc ;
	#ifdef _WIN32
	s_serial->settings.fOutxCtsFlow = false;
    s_serial->settings.fOutxDsrFlow = false;
    s_serial->settings.fDtrControl = DTR_CONTROL_DISABLE;
    s_serial->settings.fOutX = false;
    s_serial->settings.fInX = false;
    s_serial->settings.fRtsControl = RTS_CONTROL_DISABLE;
	#else
	s_serial->settings.c_iflag &= ~(IXON | IXOFF | IXANY);

	if((rc = tcsetattr(s_serial->fd, TCSANOW, &s_serial->settings)) < 0)
	{
        fprintf(stderr, "failed to set attr: %d, %s\n", s_serial->fd, strerror(errno));
        exit(EXIT_FAILURE);
    }
    #endif
	
}

#ifdef _WIN32
HANDLE s_serial_get_fd(s_serial_t * s_serial)
#else
int s_serial_get_fd(s_serial_t * s_serial)
#endif
{
	return s_serial->fd;
}

int s_serial_open(s_serial_t * s_serial,  char *comport)
{
	if(comport!=NULL)
	{
		s_serial->comport = comport;
	}
	else 
	{
		printf("Invalid port ");
		return 0;
	}
	
	#ifdef _WIN32

    s_serial->fd=CreateFileA(s_serial->comport,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

    if(s_serial->fd==INVALID_HANDLE_VALUE)
  	{
  		perror("unable to open port ");
    	return(0);
  	}

	#else

    s_serial->fd=open(s_serial->comport, O_RDWR | O_NOCTTY | O_NDELAY);

    if(s_serial->fd ==-1)
  	{
  		perror("unable to open port ");
    	return(0);
  	}
	#endif
 
 	s_serial_set_settings_default(s_serial);

  return 1;
}

void s_serial_close(s_serial_t * s_serial)
{

	#ifdef _WIN32
	CloseHandle(s_serial->fd);
	#else
	close(s_serial->fd);
	#endif

}




int s_serial_write_byte(s_serial_t * s_serial,  char byte)
{
	int n;
	#ifdef _WIN32 
	WriteFile(s_serial->fd, &byte, 1, (LPDWORD)((void *)&n), NULL);
	#else
  	n = write(s_serial->fd, &byte, 1);
  	#endif

 	if(n<0)  return(0);

  return(n);
}




int s_serial_write(s_serial_t *s_serial, char *text)  
{
	int nb=0,i=0;
	
		
  		while(text[i] != 0)   nb+=s_serial_write_byte(s_serial, text[i++]);
		
  return nb;
}




int s_serial_read(s_serial_t * s_serial,    char *buf, int size)
{
   	int n;
	#ifdef _WIN32 
	ReadFile(s_serial->fd, buf, size, (LPDWORD)((void *)&n), NULL);
	#else
	n = read(s_serial->fd, buf, size);
	#endif
  return(n);
}

