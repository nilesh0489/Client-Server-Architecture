#include	"unp.h"
#define SIZE 1024
void
str_echo(int sockfd)
{
	//long		arg1, arg2;
	ssize_t		n;
	char		buff[SIZE];

	for ( ; ; ) {
		if ( (n = Readline(sockfd, line, MAXLINE)) == 0)
                  {
                   printf("%s\n", "echo done");
			return;		/* connection closed by other end */
                   }
      
		else if(n
			snprintf(line, sizeof(line), "input error\n");
		Writen(sockfd, buff, SIZE);
	}
}
