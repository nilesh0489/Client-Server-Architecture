#include "unp.h"
#define SIZE 1024
#include <time.h>
 
void str_daytime(int sockfd)
{
     time_t ticks;
     int x,y;
     struct timeval t;
   int nread;
    fd_set readset;
    FD_ZERO(&readset);
     
  while(1)
 {
    char    buff[SIZE] = "\0";
    t.tv_sec=5;
    FD_SET(sockfd, &readset);

    x = select(sockfd + 1, &readset, NULL, NULL, &t);

   if(x == 0)
      {
         ticks = time(NULL);
          snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
       y = write(sockfd, buff, strlen(buff));

        if(y == EINTR)
         {
          printf("Client Terminated:socket read returned with value %d\n", errno);
          return;
          }
       } 

 if(x==-1)
    {
      continue;
     }

    if(FD_ISSET(sockfd, &readset))
      {
        if((y = read(sockfd, buff, strlen(buff))) == 0)
        {
          printf("%s\n", "Time client terminated successfully");
            break;
         }
      }
 } 
}
