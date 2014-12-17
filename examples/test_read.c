

#include "s_serial.h"


int main(int argc, char * argv []) 
{
	s_serial_t *s;
	int baudr=9600,n,i;
  char buf[4096];
  printf("%s\n",argv[1] );
  char * comport=argv[1];
  


	s=s_serial_new();


	if(s_serial_open(s,comport)==0)
	{
	 return 0;
	}

	s_serial_set_baudrate(s,baudr);

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
    #ifdef _WIN32
      Sleep(1000);
    #else
      usleep(1000000);  /* sleep for 1 Second */
      #endif

  }

	printf("%d\n",s->baudr );
    return 0;
}
