

#include "s_serial.h"


int main() 
{
	s_serial_t *s;
	int baudr=115200,n,i;
	unsigned char buf[4096];
	char * comport="/dev/ttyUSB0";
	char mode[]={'8','N','1',0};



	s=s_serial_new();
	if(s_serial_open(s,comport,baudr,mode)==0)
	{
		printf("Error Open Port\n");
	}
	

	  while(1)
 	 {
  		 n = s_serial_read(s, buf, 4095);
   		if(n > 0)
    		{
    		  buf[n] = 0;   /* always put a "null" at the end of a string! */

    		  for(i=0; i < n; i++)
    		  {
   		     if(buf[i] < 32)  /* replace unreadable control-codes by dots */
   		   	  {
   		   	    buf[i] = '\0';
   		   	  }
   		  	 }

      printf("received %i bytes: %s\n", n, (char *)buf);
    }
    usleep(100000);

  }

	printf("%d\n",s->baudr );
    return 0;
}
