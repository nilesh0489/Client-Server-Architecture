#include	"unp.h"
#include <netdb.h>
#define SIZE 1024

int
main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in	servaddr;
        pid_t   childpid;
        char     str [INET_ADDRSTRLEN];
        char     ipaddr [INET_ADDRSTRLEN];
        int x=0;
        int nread;
       	char buf[SIZE] = "\0";
       	char   *ptr, **pptr;
        struct hostent *hptr;
        void sig_chld(int);

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

        if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

/* gethostbyname() and gethostbyaddr() */

     if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
       {
		   ptr = argv[1];
		  if ( (hptr = gethostbyname (ptr) ) == NULL) 
		 {
		  		err_msg ("gethostbyname error for host: %s: %s",
		                ptr, hstrerror (h_errno) );
		  }
		  
		  printf ("official hostname: %s\n", hptr->h_name);

		  switch (hptr->h_addrtype) 
		{
		 	 case AF_INET:
		      		pptr = hptr->h_addr_list;
		      		for ( ; *pptr != NULL; pptr++)
		          		printf ("\taddress: %s\n",
		                  			inet_ntop (hptr->h_addrtype, *pptr, str, sizeof (str)));
		      		snprintf(ipaddr, sizeof(ipaddr), "%s", str);
		      		break;

		  	default:
		     		err_ret ("unknown address type");
		      		break;
		}
        }  // end if

     else
        {
           snprintf(ipaddr, sizeof(ipaddr), "%s", argv[1]);
           printf("IP Address : %s\n", argv[1]);

        inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
        hptr = gethostbyaddr(&servaddr.sin_addr, 4, AF_INET);
         printf("Host name: %s\n", hptr->h_name);
        } //end else


// ask the user for the type of service
	while(x !=3) 
       {                                       
          printf(" %s\n", "The services are:");
          printf(" %s\n", "1. ECHO");
          printf(" %s\n", "2. TIME");
          printf(" %s\n", "3. EXIT");
          printf(" %s\n", "Enter the choice(only numbers) you want--Eg: 1");
          printf("\n");
 
           if(scanf("%d", &x) != 1)
             {
              printf("failed to read integer.\n");
               break;
             }
          printf("\n");

//create pipe

           int pfd[2];

          if (pipe(pfd) == -1)
          {
            perror("pipe failed");
             exit(1);
          }
          
//handle the zombies
         Signal(SIGCHLD,sig_chld); 


// forking of child 
           switch(x)
           {
              case 1 :  /* echo child */
                    if ( (childpid = fork()) == 0) 
                        {
                           close(pfd[0]);
                           dup2(pfd[1], 20);
                           close(pfd[1]);
                           if ( (execlp("xterm", "xterm", "-e", "./echo_cli", ipaddr, (char *) 0)) < 0)  
                             {
                                     printf("%s\n", "Try Again");
                               exit(1);
                             } 
                        }
                    else
                  {
                    close(pfd[1]); 
                     while ((nread = read(pfd[0], buf, SIZE)) != 0)  /* parent prints the status messages received from the child */
                       printf("%s\n", buf);
                    close(pfd[0]);
                 }
                 break;

              case 2 : /* daytime child */
                    if ( (childpid = fork()) == 0) 
                        {
                           close(pfd[0]);
                           dup2(pfd[1], 20);
                           close(pfd[1]); 
                           if ( (execlp("xterm", "xterm", "-e", "./time_cli", ipaddr, (char *) 0)) < 0)  
                             {
                                     printf("%s\n", "Try Again");
                               exit(1);
                             }
                        }
                    else
                    {
                      close(pfd[1]);
                       while ((nread = read(pfd[0], buf, SIZE)) != 0)
                         printf("%s\n", buf);
                      close(pfd[0]);
                    }
                  break;

              case 3 : 
                      return;
 
              default : 
                      printf("%s\n", "Please enter the correct number");
                      break;
           } /* end switch */

      } /* end while */

	exit(0);
} /* end main */
