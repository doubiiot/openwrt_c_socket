#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <string.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <sys/shm.h>  
#include <string.h>

#define MYPORT  1234  
#define BUFFER_SIZE 1024  
char* SERVER_IP = "xxx.xxx.xxx.xxx";  
int i = 0;

void setGpio() 
{ 
		// 打开端口/sys/class/gpio# echo 16 > export
		// 设置端口方向/sys/class/gpio/gpio16# echo out > direction
		// echo 1 > enable 
		// echo 50000 > period 
		// echo 25000 > duty_cycle
    int fd; 
         fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
         if(fd == -1)
         {
			perror("enable:");      
            return ;
         }
         write(fd, "1" ,sizeof("1")); 
         close(fd); 
		
         if(i%2==0)
		 {
			 //周期
			fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
			if(fd == -1)
			{
				perror("period"); 
				return ;
			}
			write(fd, "20000000" , sizeof("20000000")); 
			printf("period success\n");
			close(fd); 

			//高电平
			fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_RDWR);
			if(fd == -1)
			{
				perror("duty_cycle");
				return ;
			}
			write(fd, "2000000" , sizeof("2000000"));
			printf("set success\n");
			close(fd); 
		 }
		 else
		 {
			fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
			if(fd == -1)
			{
				perror("period"); 
				return ;
			}
			write(fd, "20000000" , sizeof("20000000")); 
			printf("period success\n");
			close(fd); 
			
			fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_RDWR);
			if(fd == -1)
			{
				perror("duty_cycle");
				return ;
			}
			write(fd, "1000000" , sizeof("1000000"));
			printf("set success\n");
			close(fd); 
		 }
         
}  
 
int main()  
{  
    ///定义sockfd  
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);     
    ///定义sockaddr_in  
    struct sockaddr_in servaddr;  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(MYPORT);  ///服务器端口  
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);  ///服务器ip    
    printf("连接%s:%d\n",SERVER_IP,MYPORT);
	
	int fd;
	fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
         if(fd == -1)
         {
			perror("enable:");      
            return ;
         }
         write(fd, "0" ,sizeof("0")); 
         close(fd);
	
    ///连接服务器，成功返回0，错误返回-1  
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)  
    {  
        perror("connect");  
        exit(1);  
    }  
    printf("服务器连接成功\n");
	
	char sendbuf[BUFFER_SIZE];  
    char recvbuf[BUFFER_SIZE];
	char* temp = "open";
	while(1)
	{
		recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收  
		printf("从服务器接收数据：%s\n",recvbuf);
		if(strcmp(recvbuf,temp) == 0)
		{
			i++;
			setGpio();
		}
		memset(recvbuf, 0, sizeof(recvbuf));
	}
    close(sock_cli);  
    return 0;  
}  
