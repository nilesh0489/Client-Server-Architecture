#include "unp.h"
#define SIZE 1024

static void *tcpecho(void *arg);
static void *tcpdaytime(void *arg);

int LISTEN_QUEUE = 10;

void sig_handle()
{
	
	/* printf("SIGPIPE has been handled\n"); */
}

int main(int argc, char **argv)
{
	int listenfd_echo;
      int listenfd_time;
	pthread_t *pid;

       int count; /* no. of clients */

	struct sockaddr_in srv_echo, cli_echo, srv_time, cli_time;

        socklen_t clilen_echo, clilen_time;

	int *connfd_time;

	int *connfd_echo;

	int clients[FD_SETSIZE];

	int maxfd = -1;

	int maxi = 100;

        const int val = 1;

	int i = 0;

	fd_set readset;

/* handle SIGPIPE error */

	signal(SIGPIPE,sig_handle);

	/*for(i=0; i<FD_SETSIZE; i++){

		clients[i] = -1;

		}*/


/* connection to time */

	listenfd_time = Socket(AF_INET, SOCK_STREAM, 0);

        int flags;

            /* Set time socket as nonblocking */ 

       if  ( (flags = fcntl (listenfd_time, F_GETFL, 0)) < 0)
            err_sys("F_GETFL error");

         flags |= O_NONBLOCK;

       if (fcntl(listenfd_time, F_SETFL, flags) < 0)
              err_sys("F_SETFL error"); 

	bzero(&srv_time, sizeof(srv_time));

	srv_time.sin_family = AF_INET;

	srv_time.sin_addr.s_addr = htonl(INADDR_ANY);

	srv_time.sin_port = htons(4001);	

        setsockopt(listenfd_time, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

	Bind(listenfd_time, (SA*) &srv_time, sizeof(srv_time));
	
	Listen(listenfd_time, LISTEN_QUEUE); 


/* conenction to echo */

	listenfd_echo = Socket(AF_INET, SOCK_STREAM, 0);

              /* Set echo socket as nonblocking */

        if  ( (flags = fcntl (listenfd_echo, F_GETFL, 0)) < 0)
            err_sys("F_GETFL error");

         flags |= O_NONBLOCK;

        if (fcntl(listenfd_echo, F_SETFL, flags) < 0)
              err_sys("F_SETFL error"); 

	bzero(&srv_echo, sizeof(srv_echo));

	srv_echo.sin_family = AF_INET;

	srv_echo.sin_addr.s_addr = htonl(INADDR_ANY);

	srv_echo.sin_port = htons(4000);	

        setsockopt(listenfd_echo, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

	Bind(listenfd_echo, (SA *) &srv_echo, sizeof(srv_echo));

	Listen(listenfd_echo, LISTEN_QUEUE);

	if(listenfd_echo > listenfd_time)

		maxfd = listenfd_echo;

	else
	    maxfd = listenfd_time;

                maxfd = listenfd_echo;

	for ( ; ; ) 
          {
               FD_ZERO(&readset);

                if(listenfd_echo > listenfd_time)

                    maxfd = listenfd_echo;

                else

                    maxfd = listenfd_time;

		FD_SET(listenfd_echo, &readset);	

		FD_SET(listenfd_time, &readset);

		count = select(maxfd + 1, &readset, NULL, NULL, NULL);

		if(count == -1)
		{
			perror("select faulted in superserver");
			exit(1);
		}
	
		for(i=0; i < count; i++)
             {

              /* create daytime thread */

		  if(FD_ISSET(listenfd_time, &readset))
                      { 
			clilen_time = sizeof(cli_time);

			connfd_time = Malloc(sizeof(int));   
      
			if((*connfd_time = accept(listenfd_time, (SA *) &cli_time, &clilen_time)) < 0) 
                         {
                             flags &= ~O_NONBLOCK;
                             if (fcntl(listenfd_time, F_SETFL, flags) < 0)
                                 err_sys("F_SETFL error"); 

                             if(errno == EINTR || errno == ECONNABORTED)
                               {
                                  continue;
                                }
                             else
                                  printf("%s\n", "Accept Error");
                         }

			printf("Daytime thread created\n");
                        printf("\n");

			Pthread_create(&pid, NULL , &tcpdaytime, connfd_time );

		      }/*end if */   



            /* create echo thread */

		  if(FD_ISSET(listenfd_echo, &readset))
                      {   
			clilen_echo = sizeof(cli_echo);

			connfd_echo = Malloc(sizeof(int));

			if((*connfd_echo = accept(listenfd_echo, (SA *) &cli_echo, &clilen_echo)) < 0)
                        {
                             flags &= ~O_NONBLOCK;
                             if (fcntl(listenfd_echo, F_SETFL, flags) < 0)
                                 err_sys("F_SETFL error"); 

                             if(errno == EINTR || errno == ECONNABORTED)
                               {
                                  continue;
                                }
                             else
                                  printf("%s\n", "Accept Error");
                         }

			printf("Echo thread created\n");
                        printf("\n");

			Pthread_create(&pid, NULL , &tcpecho, connfd_echo );

		      }/*end if */   
	
	    }/*end for */

       }/* end infinite for */

}/* end main */



static void *tcpecho(void *arg)
 {
     int connfd;
     connfd = * ((int *) arg);
     free(arg);
     Pthread_detach(pthread_self());
     str1_echo(connfd);        
     Close(connfd);           /* done with connected socket */
     return (NULL);
 }

static void *tcpdaytime(void *arg)
 {
     int connfd1;
     connfd1 = * ((int *) arg);
     free(arg);
     Pthread_detach(pthread_self());
     str_daytime(connfd1);       
     Close(connfd1);           /* done with connected socket */
     return (NULL);
}  

