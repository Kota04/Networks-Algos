#include<iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
using namespace std;

// declaring the parameters such as port and max length of buffer
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
struct timeval timeout;
void fun(int connfd)
{
    char buff[MAX];
	int f, c, ack = 0;
	while (1)
	{
		bzero(buff, MAX);
		recv(connfd, buff, MAX, 0);
		if (strcmp("Exit", buff) == 0)
		{
			cout<<"Exit"<<endl;
			break;
		}
		f = atoi(buff);
		// checking that packet that is already recieved or not to dicard
		if (f != ack)
		{
			cout<<"Frame"<<f<<" discarded"<<endl;
			cout<<"Acknowledgement sent:"<< ack<<endl;
			bzero(buff, MAX);
			snprintf(buff, sizeof(buff), "%d", ack);
			send(connfd, buff, sizeof(buff), 0);
			continue;
		}
		c = (rand()+rand()) % 3;
		switch (c)
		{
		// packet may be lost in transmission or error occured in that packet so ack is not sent
		case 0:
			cout<<"Frame "<<f<<" not received"<<endl;
			break;
		// timeout occured for the ack or packet lost
		case 1:
			ack = f + 1;
			sleep(4);
			cout<<"Frame "<<f<<" received"<<endl;
			cout<<"Acknowledement sent: "<<ack<<endl;
			bzero(buff, MAX);
			snprintf(buff, sizeof(buff), "%d", ack);
			send(connfd, buff, sizeof(buff), 0);
			break;
			// sucessfull transmission
		case 2:
			ack = f + 1;
			cout<<"Frame "<<f<<" received"<<endl;
			cout<<"Acknowledement sent: "<< ack<<endl;
			bzero(buff, MAX);
			snprintf(buff, sizeof(buff), "%d", ack);
			send(connfd, buff, sizeof(buff), 0);
			break;
		}
	}
}

int main()
{

    int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	// creating the Socket of TCP
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd<0)
	{
	    cout<<"Error occured in the creation of the socket"<<endl;
	    exit(0);
	}
	else
	{
	    cout<<"Creation of the Socket is Socket is successfull"<<endl;
	}
	// binding the socket with the paramaters
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
	{
	    cout<<"Error occured in the Binding of the Server"<<endl;
	    exit(0);
	}
	else
	{
	    cout<<"Binding of the socket is done sucessfully"<<endl;
	}
	//listening to the requests
	if(listen(sockfd,4) != 0)
    {
        cout<<"Error occured in the listening"<<endl;
        exit(0);
    }	
    else
    {
        cout<<"Server is Listening sucessfully"<<endl;
    }
    
    // accepting the request
    len=sizeof(cli);
    connfd = accept(sockfd, (SA *)&cli, (socklen_t*)&len);
    if(connfd<0)
    {
        cout<<"Error in accepting the requests"<<endl;
        exit(0);
    }
    else
    {
        cout<<"server accepts the rquests"<<endl;
    }
    fun(connfd);
    close(sockfd);
}


