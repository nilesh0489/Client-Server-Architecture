#include "unp.h"
#define SIZE 1024

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(4000);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

        str_cli(stdin, sockfd);
        exit(0);
} /* end main */
   
 
void str_cli(FILE *fp, int sockfd)
  {
     int     maxfdp1, stdineof;
      fd_set  rset;
      char    buf[MAXLINE];
      int     n,y;
      char buf1[SIZE] = "echo server terminated prematurely";
      stdineof = 0;
     FD_ZERO(&rset);
     for ( ; ; ) 
   {
         if (stdineof == 0)
             FD_SET(fileno(fp), &rset);
         FD_SET(sockfd, &rset);
         maxfdp1 = max(fileno(fp), sockfd) + 1;
         Select(maxfdp1, &rset, NULL, NULL, NULL);

/* socket is readable */

         if (FD_ISSET(sockfd, &rset)) 
         {                                            
             if ( (n = Readline(sockfd, buf, MAXLINE)) == 0) 
           {
                 if (stdineof == 1)
                     return;       /* normal termination */
                 else
                   {
                     Write(20, buf1, strlen(buf1)+1);
                     return;
                    }
            } /* end if */

             
             if((y = write(fileno(stdout), buf, n)) < 0)
               {
                  if(errno==ECONNRESET)
                   {
                     Write(20, "ECONNRESET Error", 17);
                     return;
                   }
              } /* end if */
        }

/* input is readable */

         if (FD_ISSET(fileno(fp), &rset)) 
          { 
             if ( (n = Readline(fileno(fp), buf, MAXLINE)) == 0) 
              {
                 stdineof = 1;
                 Shutdown(sockfd, SHUT_WR);  /* send FIN */
                 FD_CLR(fileno(fp), &rset);
                 continue;
              }
             Writen(sockfd, buf, n);
          } /* end if */
               
     } /* end for */
     
 } /* end str_cli */
        
  

