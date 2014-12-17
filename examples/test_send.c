

#include "s_serial.h"

int main(int argc, char * argv []) 
{
	s_serial_t *s;

	int baudr=9600,n,i=0;

	char * comport=argv[1];
	
	char  str[2][100];

 	strcpy(str[0], "hello USB0 packet 1\n");

  	strcpy(str[1], "hello USB0 packet 2");

	
	s=s_serial_new();

	s_serial_open(s,comport);

	s_serial_set_baudrate(s,baudr);

	// s_serial_set_caractere_size(s,8);
	// s_serial_set_parity_type(s,'N');
	// s_serial_set_nb_stop_bits(s,1);


	while(1)
  	{	
    	s_serial_write(s, str[i]);

    	printf("sent: %s\n", str[i]);

    	//s_serial_set_baudr(s,9600);
	    #ifdef _WIN32
     	Sleep(1000);
   		#else
   		usleep(1000000);  /* sleep for 1 Second */
   		#endif
   		i++;

    	i %= 2;
  	}

    return 0;
}
