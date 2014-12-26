#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "s_udp.h"
#include "s_serial.h"
#include "s_loop.h"
#include "s_type.h"
#include "s_thread.h"

typedef struct _params
{
    char *portser;
    char *host;
    int port;
};

struct _params *params;

udpcli_t * cli;
udpsrv_t * srv;
s_serial_t *ser;
s_thread_t tid;
struct udpcli_info *cli_info;

char message[256];



void print_usage() 
{
    printf("Usage: -a : help  [sc] -h : host  -p port UDP  -d port Serial (/dev/tty* || COM*)  -b Baudrate  -r : parity type   -l : caractere size  -n : bits stop   -f : flow control\n");
    printf(
            "  -a           Help \n"
            "  -s           Server (by default -s) \n"
            "  -c           Client (by default -s)\n"
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
    int sys=(int)user_data;
     char buf[4096];

        printf("serudp\n");

        memset(buf,0,4095);

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

        printf("received %i bytes: %s\n", n, (char *)buf);
       if(sys==0) udp_client_send(cli, cli_info->ip, cli_info->port, buf,20);
       else udp_server_responseto(srv,buf,4095, cli_info);

    }
    
}

void udpser(int fd, void *user_data)
{
    printf("udpser\n");
    int sys=(int)user_data;
    if(sys) udp_server_recv(srv, message,256,cli_info);
    else udp_cli_recv(cli, message,256);
    printf("received <%s> from udpclient port=%d\n", message,cli_info->port);
    printf("write <%s> to device %s\n",message,params->portser);

    s_serial_write(ser, message);
    memset(message,0,256);

}


void *readblock(void *user_data)
{   
    char buf[4095];
    int n=0,i;
    int sys=(int)user_data;
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

        printf("received %i bytes: %s\n", n, (char *)buf);
      if(sys==0)  udp_client_send(cli, cli_info->ip, cli_info->port, buf,20);
      else udp_server_responseto(srv,buf,4095, cli_info);

    }
 

  }
}
int main(int argc, char *argv[]) {
   
    sloop_t * sloop_d;
    int opt= 0;
    int server = 0, client = -1,baudrate=9600,char_size=8,bstop=1,flow=-1;
    char *parity="N";
    params = (struct _params *)malloc(sizeof(struct _params));
    params->portser=NULL;
    params->host=NULL;
    params->port=0;
   

    cli_info=udpcli_info_init();
    cli=new_udp_client();
     static struct option long_options[] = {
        {"Help"     ,       no_argument         ,   0   ,  'a'  },
        {"server"   ,       no_argument         ,   0   ,  's'  },
        {"client"   ,       no_argument         ,   0   ,  'c'  },
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
    while ((opt = getopt_long(argc, argv,"asch:p:d:b:r:l:n:f:",long_options, &long_index )) != -1) 
    {
        switch (opt) {
             case 'a' : print_usage();
                        return 0;
             case 's' : server = 0;
                        break;
             case 'c' : client = 0;
                        server = -1;
                        break;
             case 'h' : cli_info->ip = optarg; 
                        break;
             case 'p' : cli_info->port = atoi(optarg);
                        break;
             case 'd' : params->portser = optarg;
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

    if ((server == -1 && client ==-1 )|| params->portser ==-1  || cli_info->port==NULL || cli_info->ip==NULL ) 
    {
        print_usage();
        exit(EXIT_FAILURE);
    }
        printf("\n\nInfo configuration");
        printf("\nUDP(port=%d,host=%s) \nserial(port=%s,baudrate=%d)   \n\n\n",cli_info->port,cli_info->ip,params->portser,baudrate);
        srv=new_udp_server(cli_info->port);
        
        ser=s_serial_new();
        sloop_d = sloop_new();
       
        if(s_serial_open(ser,params->portser)==0) return 0;
        
        s_serial_set_baudrate(ser,baudrate);
        s_serial_set_caractere_size(ser,char_size);
        s_serial_set_parity_type(ser,parity[0]);
        s_serial_set_nb_stop_bits(ser,bstop);
        sloop_set_timeout(sloop_d,1000);
        if(flow!=-1)
        if(flow) s_serial_enable_software_flow_control(ser);
        else s_serial_enable_software_flow_control(ser);
       
        #ifdef _WIN32
        

            if(client==0) 
                {
                    s_thread_create(&tid,readblock, (void*)0);
                    sloop_add_fd(sloop_d,SLOOP_RD,udp_cli_get_socket_fd(cli),udpser,(void*)0);
                }
            else 
                {
                    s_thread_create(&tid,readblock, (void*)1);
                    sloop_add_fd(sloop_d,SLOOP_RD,udp_srv_get_socket_fd(srv),udpser,(void*)1);
                }

        #else

            if(client==0)
            {
                sloop_add_fd(sloop_d,SLOOP_RD,s_serial_get_fd(ser),serudp,(void*)0);
                sloop_add_fd(sloop_d,SLOOP_RD,udp_cli_get_socket_fd(cli),udpser,(void*)0);
            }
            else
            {
                sloop_add_fd(sloop_d,SLOOP_RD,s_serial_get_fd(ser),serudp,(void*)1);
                sloop_add_fd(sloop_d,SLOOP_RD,udp_srv_get_socket_fd(srv),udpser,(void*)1);
            }
        #endif
       
        sloop_run(sloop_d);

        #ifdef _WIN32
        s_thread_join(&tid) ;
        printf("join\n");
        #endif
        

   
    return 0;
}