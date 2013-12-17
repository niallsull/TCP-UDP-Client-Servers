#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argcount, char *arv[])
{

	if(argcount!=2)
	{  // check that program + the ip was entered
		printf("two arguements, program and server ip\n");
		exit(1);
	}


	char *serverIp=arv[1];
	// record the ip address entered
	
   

	
	
	char buffer[512];  // buffer big enough to store whatever we need
	char toServer[96]="netsvr type0 nwosullivan";
// the first part of the isentifier string, can be hardcoded
// nwosullivan could have possibly been input from user

struct sockaddr_in serv_addr;
  // details of the server we'll connect to 
   serv_addr.sin_family = AF_INET;
  // serv_addr.sin_addr.s_addr = htonl(&serverIp); wrong way of doping it
   serv_addr.sin_port = htons(7531);   // hardcode the port

	inet_pton(AF_INET,serverIp,&(serv_addr.sin_addr));
// set server ip to what was entered by user


	struct sockaddr_in me;  // where i'll put details of client from sockname
	int lengthMe=sizeof(me); // and the length of me



int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
// socket, ipv4, stream, tcp
	if (sock==-1){
		printf("error creating socket");
		exit(1);
// check that the socket was made
		}

char address[30]; // to hold text string of client address

	






int connectNum=connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)); // and connect the sock to the server details




printf("\n");
if(connectNum<0)
{  // and make sure the connection happened
	printf("couldn't connect to server");
	exit(1);
}



printf("\n");


	

char buffers[512];  // another buffer created, from trying to get stuff to work
getsockname(sock,(struct sockaddr *)&me,&lengthMe);
inet_ntop(AF_INET,(struct sockaddr *)&me,address,20);
	// get ip of the client (me) details network to presentation
	printf("%s\n",address); // print out this adresss, not required
int k=sprintf(buffers,"%s %s:%d\r\n",toServer,address,ntohs(me.sin_port));

// sprintf used to bring all the details together, way easier than string concatenation

printf("client: %s\n",buffers);
ssize_t numBytes = send(sock, buffers,k, 0); // send to the auld server sure
  if (numBytes < 0){
	printf("send failes");
	exit(1);
	// and make sure we sent something

}
else if(numBytes!=k)
{
	// and that we actually sent the right amount of data
	printf("incorrect amount of data sent");
	exit(1);
}
//printf("send success");


printf("\n");
	
// do a recive while we're at it sure
	int rec=recv(sock,buffer,512,0);
	if(rec<0)
	{// and make sure we actually got data
		printf("first recv failed");
		exit(1);
	}
else
{
	printf("server: ");
	for(int i=0;i<rec;i++)
	{
	
		if(buffer[i]=='\0') // go as far as terminator
		{
			i=rec+1;
		}
		else{
			printf("%c",buffer[i]); 
		}// print out character by character
	}
	printf("\n");
}
	
	char reply[]="ok\n";
// extra send here that shouldn't be there, but couldn't get two sends in a row
	numBytes=send(sock,reply,sizeof(reply),0);
	if(numBytes<0)
	{// again check that we sent something
		printf("reply failed\n");
		exit(1);
	}
	
	printf("client: %s\n",reply);
// user sees conversation


	 rec=recv(sock,buffer,512,0);
// recieve string of bytes
	if(rec<0)
	{
		printf("second recv failed\n");
		exit(1);// check we recieved something
	}
else
{
	printf("server: ");
	for(int i=0;i<rec;i++)
	{// and print it out
		printf("%c",buffer[i]);
	}
	printf("\n");

}
	
	char num[20]; // excessly big buffer considering it's only going to be two digits long, but anyway
	

	int n=sprintf(num,"%d",rec);// slap it to printable form
// n tells us how long the string is
	numBytes=send(sock,num,n,0); // so it's usable here instead of sizeof
	if(numBytes<0)
	{
		printf("guess failed\n");
		exit(1);
	}
	printf("client: %s\n",num);

	rec=recv(sock,buffer,512,0); // and now to see if we are correct, obviously we are, why wouldn't we be 
	if(rec<0)
	{
		printf("second recv failed\n");
		exit(1);
	}
	else
	{
		printf("server: ");
		for(int i=0;i<rec;i++)
		{// print out the anwser
			printf("%c",buffer[i]);
		}
	printf("\n");

	}

    close(sock);
// and we're finished, yay, can go home now	
    exit(0);
}
