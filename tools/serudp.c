#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "s_udp.h"
#include "s_serial.h"

/** Program to calculate the area and perimeter of 
 * a rectangle using command line arguments
 */
void print_usage() {
    printf("Usage: rectangle [us]  \t -h : host \t -p port UDP \t -d port Serial (/dev/tty* || COM*) \t -b Baudrate  \t -r : parity type  \t -l : caractere size \t -n : bits stop\n");
}

int main(int argc, char *argv[]) {
    udpcli_t * cli;
    udpsrv_t * srv;
    s_serial_t *ser;

    char message[256];
    char buf[100];
    int res=0;
    int fd,n,i;
    int opt= 0;
    int udp2ser = -1, ser2udp = -1, port = -1,baudrate=115200,char_size=8,bstop=1,flow=-1;
    char *portser=NULL,*host=NULL;
    char *parity="N";
    char  str[2][100];

    struct udpcli_info *cli_info;
    cli_info=udpcli_info_init();

     static struct option long_options[] = {
        {"udp2ser"  ,       no_argument         ,   0   ,  'u'  },
        {"ser2udp"  ,       no_argument         ,   0   ,  's'  },
        {"host"     ,       required_argument   ,   0   ,  'h'  },
        {"port"     ,       required_argument   ,   0   ,  'p'  },
        {"portser"  ,       required_argument   ,   0   ,  'd'  },
        {"baudrate" ,       required_argument   ,   0   ,  'b'  },
        {"parity"   ,       required_argument   ,   0   ,  'r'  },
        {"char_size",       required_argument   ,   0   ,  'l'  },
        {"bstop"    ,       required_argument   ,   0   ,  'n'  },
        {"flow"     ,       required_argument   ,   0   ,  'f'  },
        {"msg"      ,       required_argument   ,   0   ,  'm'  },
        {0          ,       0                   ,   0   ,   0   }
    };

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"ush:p:d:b:r:l:n:f:m:",long_options, &long_index )) != -1) 
    {
        switch (opt) {
             case 'u' : udp2ser = 0;
                 break;
             case 's' : ser2udp = 0;
                 break;
             case 'h' : host = optarg; 
                 break;
             case 'p' : port = atoi(optarg);
                 break;
             case 'd' : portser = optarg;
                 break;
             case 'b' : baudrate = atoi(optarg);
                 break;
             case 'r' : parity = optarg;
                        printf("%c",parity[0]);
                 break;
             case 'l' : char_size = atoi(optarg);
                 break;
             case 'n' : bstop = atoi(optarg);
                 break;
             case 'f' : flow = atoi(optarg);
                 break;
             case 'm' : memset(str, 0, sizeof(str)); 

                        strcat(str[0], argv[--optind]);
                        optind++;

                        while (optind < argc)
                        {
                            strcat(str[0], " ");
                            strcat(str[0], argv[optind++]);                   
                        }
               
                 break;
             default: print_usage(); 
                 exit(EXIT_FAILURE);
        }
    }
    if ((udp2ser == -1 && ser2udp ==-1 )|| port ==-1  || baudrate ==-1 || portser==NULL || host==NULL ) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    if (udp2ser == 0) 
    {   
        printf("\n\nUDP(port=%d,host=%s) to serial(port=%s,baudrate=%d)   \n",port,host,portser,baudrate);

        srv=new_udp_server(port);
        ser=s_serial_new();
        if(s_serial_open(ser,portser)==0)
        {
         return 0;
        }
        s_serial_set_baudrate(ser,baudrate);
        s_serial_set_caractere_size(ser,char_size);
        s_serial_set_parity_type(ser,parity[0]);
        s_serial_set_nb_stop_bits(ser,bstop);
        while(1)
        {
        udp_server_recv(srv, message,256,cli_info);

        printf("received <%s> from udpclient port=%d\n", message,port);
        printf("write <%s> to device %s\n",message,portser);
        s_serial_write(ser, message);
        //res=udp_server_responseto(srv,"well received\r\n",strlen("well received\r\n"), cli_info);
        bzero(message,256);

        }

    }

    if (ser2udp == 0) 
    {
    
        printf("\n\nserial(port=%s,baudrate=%d) to UDP(port=%d,host=%s) \n",portser,baudrate,port,host);
       
        ser=s_serial_new();
        cli=new_udp_client();

        if(s_serial_open(ser,portser)==0) return 0;
     
        s_serial_set_baudrate(ser,baudrate);
        s_serial_set_caractere_size(ser,char_size);
        s_serial_set_parity_type(ser,parity[0]);
        s_serial_set_nb_stop_bits(ser,bstop);
        while(1)
        {
            n = s_serial_read(ser, buf, 4095);
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

                printf("received %i bytes: <%s> from %s\n", n, (char *)buf,portser);
                printf("sent <%s> to udpserver port= %d\n",(char *)buf,port );

            udp_client_send(cli, host, port, buf,20);
            }
        #ifdef _WIN32
         Sleep(1000);
        #else
         usleep(1000000);  /* sleep for 1 Second */
        #endif

        }
       

    }
    return 0;
}