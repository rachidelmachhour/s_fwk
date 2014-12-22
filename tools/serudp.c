#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "s_udp.h"
#include "s_serial.h"
#include "s_loop.h"

udpcli_t * cli;
udpsrv_t * srv;
s_serial_t *ser;
struct udpcli_info *cli_info;

char message[256];

char *portser=NULL,*host=NULL;
int port=-1;

void print_usage() 
{
    printf("Usage:  [us] -h : host  -p port UDP  -d port Serial (/dev/tty* || COM*)  -b Baudrate  -r : parity type   -l : caractere size  -n : bits stop   -f : flow control\n");
    printf(
            "  -u           UDP to SERIAL (by default -u) \n"
            "  -s           SERIAL to UDP (by default -u)\n"
            "  -h           host\n"
            "  -p           port\n"
            "  -d           port serial (/dev/tty* || COM*)\n"
            "  -b           Baudrate \n"
            "  -r           parity type(N || E || O) \n"
            "  -l           caractere size(8 || 7 || 6 || 5) \n"
            "  -n           nb bits stop ( 1 || 2 ) \n"
            "  -f           flow control (0 : hardware || 1 : software) \n"
            );
}


void serudp(int fd, void *user_data)
{
    int n , i;
    char buf[100];

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
            udp_client_send(cli, host, port, buf,100);
        }
    
}

void udpser(int fd, void *user_data)
{

    udp_server_recv(srv, message,256,cli_info);

    printf("received <%s> from udpclient port=%d\n", message,port);
    printf("write <%s> to device %s\n",message,portser);
    s_serial_write(ser, message);
    memset(message,0,256);

}


int main(int argc, char *argv[]) {
    
    sloop_t * sloop_d;
    
    char buf[100];
    int res=0;
    int fd,n,i;
    int opt= 0;
    int udp2ser = 0, ser2udp = -1,baudrate=115200,char_size=8,bstop=1,flow=-1;
  
    char *parity="N";
    char  str[2][100];

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
        {0          ,       0                   ,   0   ,   0   }
    };

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"ush:p:d:b:r:l:n:f:",long_options, &long_index )) != -1) 
    {
        switch (opt) {
             case 'u' : udp2ser = 0;
                        break;
             case 's' : ser2udp = 0;
                        udp2ser = -1;
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
                        break;
             case 'l' : char_size = atoi(optarg);
                        break;
             case 'n' : bstop = atoi(optarg);
                        break;
             case 'f' : flow = atoi(optarg);
                        break;
            
             default: print_usage(); 
                 exit(EXIT_FAILURE);
        }
    }
    if ((udp2ser == -1 && ser2udp ==-1 )|| port ==-1  || portser==NULL || host==NULL ) 
    {
        print_usage();
        exit(EXIT_FAILURE);
    }

    if (udp2ser == 0) 
    {   
        printf("\n\nUDP(port=%d,host=%s) to serial(port=%s,baudrate=%d)   \n",port,host,portser,baudrate);

        srv=new_udp_server(port);
        ser=s_serial_new();
        sloop_d = sloop_new();

        if(s_serial_open(ser,portser)==0) return 0;
        
        s_serial_set_baudrate(ser,baudrate);
        s_serial_set_caractere_size(ser,char_size);
        s_serial_set_parity_type(ser,parity[0]);
        s_serial_set_nb_stop_bits(ser,bstop);
       
        if(flow!=-1)
        if(flow) s_serial_enable_software_flow_control(ser);
        else s_serial_enable_software_flow_control(ser);

        
        sloop_set_timeout(sloop_d,1000);

        sloop_add_fd(sloop_d,SLOOP_RD,udp_srv_get_socket_fd(srv),udpser,NULL);

        sloop_run(sloop_d);

    }

    if (ser2udp == 0) 
    {
    
        printf("\n\nserial(port=%s,baudrate=%d) to UDP(port=%d,host=%s) \n",portser,baudrate,port,host);
       
        ser=s_serial_new();
        cli=new_udp_client();
        sloop_d = sloop_new();

        if(s_serial_open(ser,portser)==0) return 0;
     
        s_serial_set_baudrate(ser,baudrate);
        s_serial_set_caractere_size(ser,char_size);
        s_serial_set_parity_type(ser,parity[0]);
        s_serial_set_nb_stop_bits(ser,bstop);
       
        printf("%d\n",s_serial_get_fd(ser) );
        
        sloop_set_timeout(sloop_d,1000);


        sloop_add_fd(sloop_d,SLOOP_RD,s_serial_get_fd(ser),serudp,NULL);
        
        sloop_run(sloop_d);

       

    }
    return 0;
}