#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argcount, char *arv[])
{

	if(argcount!=2) // check they put in something for an ip address
	{
		printf("two arguements, program and server ip\n");
		exit(1);
	}


	char *serverIp=arv[1]; // hold the address

	char address[20];
	char buffer[512];
	
	int sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	// create our socket, ipv4, datagram,udp
	if(sock<0)
	{
		printf("socket failed\n");
		exit(1);// check socket was created
	}



	struct sockaddr_in serverAddress;// hold server details

	serverAddress.sin_family=AF_INET;
	//serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(7531);
	int svrlen=sizeof(serverAddress);

	inet_pton(AF_INET,serverIp,&(serverAddress.sin_addr));
// and put in the entered ip address
	

	struct sockaddr_storage clntAddr;
	socklen_t cliLen=sizeof(clntAddr);

	struct sockaddr_in me;
	int len = sizeof(me);


	int con=connect(sock,(struct sockaddr *)&serverAddress,sizeof(serverAddress)); // connect to the socket

	getsockname(sock,(struct sockaddr *)&me,&len);
	//get details of client
	inet_ntop(AF_INET,(struct sockaddr *)&me,address,20);
	//and put into address holder
	char toServer[]="netsvr type1 nwosullivan";

	//inet_ntop(AF_INET,(struct sockaddr*)&serverAddress,address,20);

int size=sprintf(buffer,"%s %s:%d\r\n",toServer,address,ntohs(me.sin_port)); // bring it altogether

	ssize_t sent=sendto(sock,buffer,size,0,(struct sockaddr *) &serverAddress,svrlen); // and send identifier

	if(sent<0)
	{// check we sent something
		printf("send failed\n");
		exit(1);
	}
	printf("client: %s\n",buffer);


	ssize_t rec=recvfrom(sock,buffer,512,0,(struct sockaddr *) &clntAddr,&cliLen); // recieve welcome

	if(rec<0)
	{ // check recieve
		printf("recv failed\n");
		exit(1);
	}
	else
	{
		printf("server: ");
		for(int i=0;i<rec;i++)
		{
			printf("%c",buffer[i]);
		}
		printf("\n");
	}

	char reply[]="ok";

	// extra send
	sent=sendto(sock,reply,sizeof(reply),0,(struct sockaddr *) &serverAddress,svrlen);

	if(sent<0)
	{// check send
		printf("send failed\n");
		exit(1);
	}
	printf("client: %s\n",reply);
	
	rec=recvfrom(sock,buffer,512,0,(struct sockaddr *) &clntAddr,&cliLen); // recieve string of a's  random bytes

	if(rec<0)
	{// check recieve
		printf("recv failed\n");
		exit(1);
	}
	printf("server: ");
	for(int i=0;i<rec;i++)
	{
		printf("%c",buffer[i]);
	}
	printf("\n");


	int n=sprintf(buffer,"%d",rec);
// get number recieved in readable form
	
	
	sent=sendto(sock,buffer,n,0,(struct sockaddr *) &serverAddress,svrlen);
// send our answer to the server
	if(sent<0)
	{// chack send
		printf("send failed\n");
		exit(1);
	}
	printf("client: %s\n",buffer);

	rec=recvfrom(sock,buffer,512,0,(struct sockaddr *) &clntAddr,&cliLen);// recieve to check it we're right

	if(rec<0)
	{ // check recieve
		printf("recv failed\n");
		exit(1);
	}
	

	printf("server: ");
	for(int i=0;i<rec;i++)
	{

		printf("%c",buffer[i]);
	}


	printf("\n");


	close(sock);
	return(0);
}
