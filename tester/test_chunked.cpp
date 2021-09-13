#include<stdio.h>	//printf
#include<string.h>	//strlen
#include<sys/socket.h>	//socket
#include<sys/time.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//usleep
#include<fcntl.h>	//fcntl
#include<string>

//Size of each chunk of data received, try changing this
#define CHUNK_SIZE 512

//Receiving function
int recv_timeout(int s, int timeout);

int main()
{
	int socket_desc;
	struct sockaddr_in server;
	std::string message;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	
	//ip address of www.msn.com (get by doing a ping www.msn.com at terminal)
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 9000 );

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	
	puts("Connected");
	
	//Send some data
	message = "GET / HTTP/1.1\r\nHost: 127.0.0.98\r\n";

	if( send(socket_desc , message.c_str() , strlen(message.c_str()) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send");
	
	//Now receive full data
	int total_recv = recv_timeout(socket_desc, 4);
	
	printf("Done. Received a total of %d bytes\n\n" , total_recv);
	return 0;
}

/*
	Receive data in multiple chunks by checking a non-blocking socket
	Timeout in seconds
*/
int recv_timeout(int s , int timeout)
{
	int size_recv , total_size= 0;
	struct timeval begin , now;
	char chunk[CHUNK_SIZE];
	double timediff;
	
	//make socket non blocking
	fcntl(s, F_SETFL, O_NONBLOCK);
	
	//beginning time
	gettimeofday(&begin , NULL);

	while(1)
	{
		gettimeofday(&now , NULL);
		
		//time elapsed in seconds
		timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
		
		//if you got some data, then break after timeout
		if( total_size > 0 && timediff > timeout )
		{
			break;
		}
		
		//if you got no data at all, wait a little longer, twice the timeout
		else if( timediff > timeout*2)
		{
			break;
		}
		
		memset(chunk ,0 , CHUNK_SIZE);	//clear the variable
		if((size_recv =  recv(s , chunk , CHUNK_SIZE , 0) ) < 0)
		{
			//if nothing was received then we want to wait a little before trying again, 0.1 seconds
			usleep(100000);
		}
		else
		{
			total_size += size_recv;
			printf("%s" , chunk);
			//reset beginning time
			gettimeofday(&begin , NULL);
		}
	}
	
	return total_size;
}
