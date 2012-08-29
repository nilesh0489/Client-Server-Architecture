 #include    "unp.h"
 #define SIZE 1024

 void str1_echo(int sockfd)
{
     int n=0;

  while(1)
   { 
     char buff[SIZE]="\0";
     n = readline(sockfd, buff, SIZE);

     if(n>0)
         Writen(sockfd, buff, n);

     if(n == 0)
       {
        printf("%s\n", "Echo Client Terminated Successfully");
        printf("\n");
        return;
       }

     if (errno == EINTR)
          { 
            printf("%s\n", "Client Terminated:socket read returned with value 0");
            printf("\n");
            return;
           }
           
  /*   if (n < 0)
         { 
            printf("%s\n", "Client Terminated:socket read returned with value -1");
            printf("\n");
            return;
           }          */
           
  } /* end while */

} /* end str_echo */
