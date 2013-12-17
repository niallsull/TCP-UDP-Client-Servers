#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <time.h>
int main(int argcount, char *arv[])
{
   
	



	

	

	int sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// create our socket for the server


	if(sock<0)
	{
		printf( "socket creation failed  Beware.\n" );
	// check it was created successfully
		exit(1);
	}
else{

	printf("socket success\n");
}


// details of the server
struct sockaddr_in servAddr;                   //internet socket address structure
  memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
  servAddr.sin_family = AF_INET;                // IPv4 address family
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // conect to local interface; host to network long[integer]
  servAddr.sin_port = htons(7531);          // hardcode to 7531

	int bindResult=bind(sock,(struct sockaddr*) &servAddr,sizeof(servAddr));// bind details to the socket


	if(bindResult<0)
	{
// and check bound successfully
		printf( "bound failed \n" );
		exit(1);
	}
else
{
printf("bind success\n");
}
	int listenResult=listen(sock,2);
// listen for clients, max two, but closes anywaty, not in a loop
	if(listenResult<0)
	{
// check that we listened
		printf( "listen failed \n" );
		exit(1);
	}

printf("listen success\n");


struct sockaddr_in clntAddr; 
  socklen_t clntAddrLen = sizeof(clntAddr);
// create empty structure to put incoming client into;
  
  int clntSock = accept(sock, (struct sockaddr *) &clntAddr, &clntAddrLen);// and accept that client
  	if (clntSock < 0)
  	 {
		printf("accept failed\n");
		exit(1);
	// and check we accepted the client
	}

	

	printf("accept success\n");

	
	char buffer[512];
	// buffer to hold data
	printf("before recieve\n");

ssize_t rec=recv(clntSock,buffer,512,0);
// do the first recieve from client
	if(rec<0)
{// check it worked
	printf("recieve failed");
	exit(1);
}

else
{
	printf("client: ");// show convo to the user
	for(int i=0;i<rec;i++)
	{
		printf("%c",buffer[i]);// print out characters that were recieved
	}
	printf("\n");
}


	char net[6]; // create holder for each part of the string
	strncpy(net,buffer,6); // and copy data from buffer to holder
	net[6]='\0';// and add terminator
	printf("%s\n",net);
	char type[5];// repeat
	strncpy(type,buffer+7,5);
	type[5]='\0';
	//printf("%s\n",type);
	int i=13;
	int s=0;// used to determine length of word
	while(buffer[i]!=' ')// for the name we don't know the size
	{// so we have to calculate how long the name is
		i++;
		s++;
	}// by going to the next white space

	char name[(i-13)]; // then give it the correct size
	strncpy(name,buffer+13,s);// and correct start and end
	name[(i-13)]='\0';
	//printf("%s\n",name);

	i++;
	s=0;
	int start=i;  // repeat again for ip address
	while(buffer[i]!=':')// no white space this time but ':'
	{
		i++;
		s++;
	}
	char ipAddr[(i-start)];
	strncpy(ipAddr,buffer+start,s);
	ipAddr[(i-start)]='\0';
	//printf("%s\n",ipAddr);

	i++;
	s=0;
	start=i;
	
	
	// for the port number we keep going once we have numbers, no whitespace left just \r\n
	while( (buffer[i]=='1') || (buffer[i]=='2') || (buffer[i]=='3') || (buffer[i]=='4') || (buffer[i]=='5') || (buffer[i]=='6') || (buffer[i]=='7') || (buffer[i]=='8') || (buffer[i]=='9') || (buffer[i]=='0') )
{
	i++;
}
	// and make port no. holder
	char portNum[(i-start)];
	strncpy(portNum,buffer+start,i-start);
	portNum[(i-start)]='\0';
	//printf("%s\n",portNum);
	int t=0;

//printf("size of potnum is %d\n",sizeof(portNum));
while(t<sizeof(ipAddr))
{
	if((ipAddr[t]== '1') || (ipAddr[t]=='2') || (ipAddr[t]=='3') || (ipAddr[t]=='4') || (ipAddr[t]=='5') || (ipAddr[t]=='6') || (ipAddr[t]=='7') || (ipAddr[t]=='8') || (ipAddr[t]=='9') || (ipAddr[t]=='0') || (ipAddr[t]=='.'))
{ // check that ip address has valid data, we could check how many dots were entered and the distance between each dot, but really beyond what we are trying to achieve for simplicity
//printf("%d",t);
	t++;
}
else
{
	printf("invalid ipaddress\n");
	exit(1);
}
}

t=0;



while(t<sizeof(portNum))
{
	if((portNum[t]== '1') || (portNum[t]=='2') || (portNum[t]=='3') || (portNum[t]=='4') || (portNum[t]=='5') || (portNum[t]=='6') || (portNum[t]=='7') || (portNum[t]=='8') || (portNum[t]=='9') || (portNum[t]=='0'))
{ // and checking port number we just check for valid numbers
//printf("%d",t);
	t++;
}
else
{
	printf("invalid portNumber\n");
	exit(1);
}
}

	

	char message1[96]; // storage for dirst meassage
	int a=sprintf(message1,"%s %s:%s %s\r\n\0","hello",ipAddr,portNum,"welcome to the netsvr server");
//sprintf to bring all together and calculate length
	srand(time(NULL));// how we'll get our random bytes
	int r = (rand()%50)+1; // max of 50, min of 1
	
char message2[r];

	for(int i=0;i<r;i++)
	{
		message2[i]='a';// put the a's into message two
	}
	
	printf("server: %s\n",message1);
	ssize_t numSent=send(clntSock,message1,a,0);
	if(numSent<0)
	{ // send back our welcome message
		printf("send failed");
		exit(1); // and check we sent
	}
	
	rec=recv(clntSock,buffer,512,0);
	//extra recieve , 
	if(rec<0)
	{
		printf("recieve failed");
		exit(1);
	}

	else
	{
		printf("client: ");
		for(int i=0;i<rec;i++)
		{
			printf("%c",buffer[i]);
		}
		printf("\n");

	}
	// send on our random bytes
	printf("server: %s\n ",message2);
	numSent=send(clntSock,message2,sizeof(message2),0);
	if(numSent<0)
	{// and check we sent
		printf("send failed");
		exit(1);
	}

	int recd=recv(clntSock,buffer,512,0);
	//recieve the answer
	if(rec<0)
{ // check we recieved
	printf("recieve failed\n");
	exit(1);
}

else
{
	printf("client: ");
	for(int i=0;i<recd;i++)
	{
		printf("%c",buffer[i]);
	}
	printf("\n");
}
	
	//printf("verifying answer\n");
	int y=atoi(buffer);// convert clients answer back to integer

	char cookie[9]; // chocolate chip cookie
	char num[1]; // was meant to be used

	for(int c=0;c<9;c++)
	{
		int r=(rand()%1000000000)+1; // high random number
		sprintf(cookie,"%d",r); // put in printable form
		
		
	}

	if(y==r) // check they were correct
	{
		char answer[]="correct";// obv
		int c=sprintf(buffer,"%s %s",answer,cookie);// put together
		numSent=send(clntSock,buffer,c,0);// send answer and cookie
		printf("server: %s\n",buffer);
	}
	else
	{
		char answer[]="wrong"; // same as above
		int c=sprintf(buffer,"%s %s",answer,cookie);
		numSent=send(clntSock,buffer,c,0);
		printf("server: %s\n",buffer);
	}

	if(numSent<0)
	{
		printf("verify send failed\n");
		exit(1);
	}

    close(clntSock);
close(sock);
    exit(0);
}
