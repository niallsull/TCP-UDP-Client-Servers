#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>

int main()
{

	char buffer[512];
	char toClient[]="hello";
	
	int sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
// cretae socket
	if(sock<0)
	{//check creation
		printf("socket failed\n");
		exit(1);
	}

	
	struct sockaddr_in serverAddress;

	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(7531);

	// server details, on whatever ip it can find

	int bound=bind(sock,(struct sockaddr *) &serverAddress,sizeof(serverAddress));
// bindthe sock to the server details

	struct sockaddr_storage clntAddr;
	socklen_t cliLen=sizeof(clntAddr);// create struct to hold client

	ssize_t rec=recvfrom(sock,buffer,512,0,(struct sockaddr *) &clntAddr,&cliLen);
//recieve welcome message
	if(rec<0)
	{// check recieve
		printf("recv failed\n");
		exit(1);
	}

	else
	{
		printf("client: ");//user sees conversation
		for(int i=0;i<rec;i++)
		{
			printf("%c",buffer[i]);//print out what was recievd
		}
		printf("\n");
	}
// split up the string recieved
// the first two we know wht's in them
	char net[6];
	strncpy(net,buffer,6);
	net[6]='\0';
	//printf("%s\n",net);
	char type[5];
	strncpy(type,buffer+7,5);
	type[5]='\0';
	//printf("%s\n",type);
	int i=13;
	int s=0; // for how long next word is
// for name ip and port, we check for whitespace etc, to determine iy's length
	while(buffer[i]!=' ')
	{
		i++;
		s++;
	}

	char name[(i-13)];
	strncpy(name,buffer+13,s);
	name[(i-13)]='\0';
	//printf("%s\n",name);

	i++;
	s=0;
	int start=i;
	while(buffer[i]!=':')// ip ends at ':' not ' '
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

	while( (buffer[i]=='1') || (buffer[i]=='2') || (buffer[i]=='3') || (buffer[i]=='4') || (buffer[i]=='5') || (buffer[i]=='6') || (buffer[i]=='7') || (buffer[i]=='8') || (buffer[i]=='9') || (buffer[i]=='0') )
{
	i++;
}// port num ends once no valid numbers left
	
	char portNum[(i-start)];
	strncpy(portNum,buffer+start,i-start);
	portNum[(i-start)]='\0'; 
	//printf("%s\n",portNum);
	



int t=0;
while(t<sizeof(ipAddr))
{
	if((ipAddr[t]== '1') || (ipAddr[t]=='2') || (ipAddr[t]=='3') || (ipAddr[t]=='4') || (ipAddr[t]=='5') || (ipAddr[t]=='6') || (ipAddr[t]=='7') || (ipAddr[t]=='8') || (ipAddr[t]=='9') || (ipAddr[t]=='0') || (ipAddr[t]=='.'))
{// check ipaddre for all numbers + '.'
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
{// and check port number for all numbers
	if((portNum[t]== '1') || (portNum[t]=='2') || (portNum[t]=='3') || (portNum[t]=='4') || (portNum[t]=='5') || (portNum[t]=='6') || (portNum[t]=='7') || (portNum[t]=='8') || (portNum[t]=='9') || (portNum[t]=='0') )
{
//printf("%d",t);
	t++;
}
else
{
	printf("invalid portNumber\n");
	exit(1);
}
}




	s=sprintf(buffer,"%s %s:%s %s\r\n","hello",ipAddr,portNum,"welcome to the netsvr server");	
	// bring everything together and calc length
	ssize_t sent=sendto(sock,buffer,s,0,(struct sockaddr *) &clntAddr,cliLen); // send welcome string

	if(sent<0)
	{//check send
		printf("send failed\n");
		exit(1);
	}
	printf("server: %s\n",buffer);



	rec=recvfrom(sock,buffer,512,0,(struct sockaddr *) &clntAddr,&cliLen);// recieve the ok

	if(rec<0)
	{// check recieve
		printf("recv failed\n");
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


//random time

	srand(time(NULL));// for random bytes
	int random=(rand()%50)+1; // 1-50
	char bytes[random];
	for(int i=0;i<random;i++)
	{
		bytes[i]='a';// fill buffer with a's
	}

	sprintf(buffer,"%s\n",bytes);
	sent=sendto(sock,buffer,random,0,(struct sockaddr *) &clntAddr,cliLen);// send random length string

	if(sent<0)
	{//check send
		printf("send failed\n");
		exit(1);
	}
	printf("server: %s\n",buffer);

	rec=recvfrom(sock,buffer,512,0,(struct sockaddr *) &clntAddr,&cliLen); // recieve clients answer

	if(rec<0)
	{//check recieve
		printf("recv failed\n");
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

	//printf("verifying answer\n");
	i=atoi(buffer);
	// put answer into integer form



	char cookie[9]; // cookies make you fat, espicially when this big
	char num[1];

	for(int c=0;c<9;c++)
	{
		int r=(rand()%1000000000)+1;
		sprintf(cookie,"%d",r);//put fat cookie in readable form
		
		
	}

	
	
	if(i==random)
	{//check answer  is correct
		char answer[]="correct";
		int c=sprintf(buffer,"%s %s",answer,cookie);
		sent=sendto(sock,buffer,c,0,(struct sockaddr *) &clntAddr,cliLen);// put answer and cookie together, tell client
	printf("server: %s\n",buffer);
	}
	else
	{
		char answer[]="wrong";
		int c=sprintf(buffer,"%s %s",answer,cookie);
		sent=sendto(sock,buffer,c,0,(struct sockaddr *) &clntAddr,cliLen);  // same as above
	printf("server: %s\n",buffer);
	}

	if(sent<0)
	{//check send
		printf("verify send failed\n");
		exit(1);
	}



	close(sock);
	return 0;
// go home and have the xmas dinner, have that cookie for dessert

}
