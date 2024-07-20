#include<iostream>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
struct timeval timeout;
void func(int sockfd, int nf, int ws)
{
	char buff[MAX];
	int ack, i = 0, n, k;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0)
	{
		cout<<"setsockopt(SO_RCVTIMEO) failed"<<endl;
		exit(0);
	}
	cout<<"Frame "<<i<<" sent"<<endl;
	bzero(buff, sizeof(buff));
	snprintf(buff, sizeof(buff), "%d", i);
	k = send(sockfd, buff, sizeof(buff), 0);
	while (1)
	{
		bzero(buff, sizeof(buff));
		n = recv(sockfd, buff, MAX, 0);
		ack = atoi(buff);
		if (n > 0)
		{
		    // sending the last frame
			if (ack == nf)
			{
				cout<<"Acknowlegement received: "<<ack<<endl;
			    cout<<"Exit"<<endl;
				bzero(buff, sizeof(buff));
				strcpy(buff, "Exit");
				k = send(sockfd, buff, sizeof(buff), 0);
				break;
			}
			// sucessfull transmission
			if (ack == i + 1)
			{
				i++;
				cout<<"Acknowlegement received: "<<i<<endl;
				cout<<"Frame "<< ack<<" sent"<<endl;
				bzero(buff, sizeof(buff));
				snprintf(buff, sizeof(buff), "%d", i);
				k = send(sockfd, buff, sizeof(buff), 0);
			}
		}
		// faiure in the transmisssion
		else
		{
			cout<<"Acknowledgement "<<i+1<<" not received"<<endl;
			cout<<"Resending frame "<<i<<endl;
			bzero(buff, sizeof(buff));
			snprintf(buff, sizeof(buff), "%d", i);
			k = send(sockfd, buff, sizeof(buff), 0);
		}
	}
}
int main()
{
	int sockfd, connfd, f, w;
	struct sockaddr_in servaddr, cli;
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
	// binding the paramaters
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
	{
		cout<<"Connection with the server failed"<<endl;
		exit(0);
	}
	else
	{
		cout<<"Connected to the server"<<endl;
	}
	cout<<"Enter the number of frames: ";
	cin>>f;
	func(sockfd, f, 1);
	close(sockfd);
	return 0;
}
