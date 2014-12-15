

#include "s_serial.h"

int main() 

{
	s_serial_t *s;
	int baudr=115200,n,i;
	unsigned char buf[4096];
	char * comport="/dev/ttyUSB1";
	char mode[]={'8','N','1',0};
	char  str[2][4096];

  	strcpy(str[0], "hello USB0 packet 1\n");

  	strcpy(str[1], "hello USB111 packet 2");



	s=s_serial_new();

	s_serial_open(s,comport,baudr,mode);

	while(1)
  	{
    	s_serial_write(s, str[i]);

    	printf("sent: %s\n", str[i]);

	    usleep(1000000);  /* sleep for 1 Second */

    	i++;

    	i %= 2;
  	}


	printf("%d\n",s->baudr );
    return 0;
}
