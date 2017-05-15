#include <stdio.h>
#include <sys/types.h>  //for socket
#include <sys/socket.h> //for sockets
#include <netinet/in.h>  //for the structures
#include <string.h>   //for strings
#include <stdlib.h>
#include <errno.h> // for perror
#include <arpa/inet.h>
int connection = 0;



void establishConnection(x)
{
	char msgUsername[1024]="";
	char msg[4096]="";
	if(x > 1)
	{
		goto options;
	}

	int sock;
	int sent;
	int recvd;
	int connectsock;
	unsigned int len;
	char buffer[1024];
	char buffer1[1024]= "";
	char username[10] = "";
	char password[10] = "";
	char welcomeMessage[50];
	char ip[16];
	int port;
	char endconv[] = "bye\n";
	struct sockaddr_in server, client;
	len = sizeof(struct sockaddr_in);
	
	//check if connection is already established
	if(connection == 1)
	{
		printf("=======You are already logged in=======\n");
		return;
	}


	//if connection is not established then ask for IP and port
	printf("Please enter the IP address: ");
	scanf("%15s", ip);
	printf("Please enter Port Number: ");
    scanf("%d",&port);
	//establish socket()
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error in Socket Creating");
		exit(-1);
	}

	bzero(&server, len);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr =inet_addr(ip);
	
	//establish connect()
	connectsock = connect(sock, (struct sockaddr *) &server, sizeof(server));
	if (connectsock == -1)
	{
		perror("Error while connecting");
		exit(-1);
	}

	printf("-----------------------------\n");
	printf("Connecting to server\n");
	sent = send(sock, "1" , 1,0);
	if(sent == -1)
	{
	 	perror("No messga Sent");
		exit(-1);
	}


	//ask for credentials
	printf("Please enter your username: ");
	scanf("%s", username);


  	//send
   sent = send(sock, username, strlen(username),0);
   if(sent == -1)
   {
   		perror("No username Sent");
   		exit(-1);
   }

	printf("Please enter your password: ");
	scanf("%s", password);


  	//send
   sent = send(sock, password, strlen(password),0);
   if(sent == -1)
   {
   		perror("No password Sent");
   		exit(-1);
   }

   //recieve welcome message
   recvd = recv(sock, welcomeMessage, 50,0);
   if(recvd == -1)
   {
   		perror("No Welcome Message Recieved");
   		exit(-1);
   }

   	printf("%s\n",welcomeMessage);

	connection = 1;
	
	options:;
	printf("%d", x);


	//get users if option is 2
	if(x == 2)
	{
		printf("-----------------------------\n");
		printf("Getting User List\n");
		sent = send(sock, "2" , 1,0);
	    if(sent == -1)
	    {
	   	  	perror("No messga Sent");
	   		exit(-1);
	    }

	    recvd = recv(sock, buffer, 15,0);
   		if(recvd == -1)
   		{
   			perror("No Welcome Message Recieved");
   			exit(-1);
   		}
   		printf("User List:\n%s\n\n\n",buffer );


		return;
	}



	//send an offline message if user option is 3
	if(x == 3)
	{
		sent = send(sock, "3" , 1,0);
	    if(sent == -1)
	    {
	   	  	perror("No messga Sent");
	   		exit(-1);
	    }
		printf("-----------------------------\n");
		printf("Send a message\n");
		printf("Please Enter the username of your friend: ");
		scanf("%s", msgUsername);
		printf("Please enter your message: ");
		getchar();
		fgets(msg, sizeof(msg), stdin);
		printf("%s\n",msg );
		sent = send(sock, msgUsername , 10,0);
	    if(sent == -1)
	    {
	   	  	perror("No messga Sent");
	   		exit(-1);
	    }

	   	sent = send(sock, msg , 100,0);
	    if(sent == -1)
	    {
	   	  	perror("No messga Sent");
	   		exit(-1);
	    }

		return;
	}



	//read messages dropped for you when offline
	if(x == 4)
	{
		printf("-----------------------------\n");
		printf("Get my messages\n");
		sent = send(sock, "4" , 1,0);
	    if(sent == -1)
	    {
	   	  	perror("No messga Sent");
	   		exit(-1);
	    }

	    recvd = recv(sock, buffer1, 1024,0);
   		if(recvd == -1)
   		{
   			perror("No Message Recieved");
   			exit(-1);
   		}
		printf("Messages: %s\n",buffer1 );
		return;
	}



	//listen on a port to chat with a firned
	if(x == 5)
	{
		printf("-----------------------------\n");
		printf("Initiate a chat with a friend\n");
		sent = send(sock, "5" , 1,0);
	    if(sent == -1)
	    {
	   	  	perror("No messga Sent");
	   		exit(-1);
	    }
	    connection == 1;
	    printf("Disconnecting from the server\n\n\n");
	    close(sock);


		//socket()
		int sock, bindsock, listensock, acceptsock;
		unsigned int len;
		int recvsock, sendsock;
		char msg[4096];
		int listeningPort;

		struct sockaddr_in server, client;
		len = sizeof(struct sockaddr_in);

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == -1)
		{
		  perror("Socket: ");
		  exit(-1);
		}


		printf("Please enter Port Number: ");
    	scanf("%d",&listeningPort);


		bzero(&server, len);
		server.sin_family = AF_INET;
		server.sin_port = htons(listeningPort);
		server.sin_addr.s_addr = INADDR_ANY;


		//bind()


		bindsock = bind(sock, (struct sockaddr *)&server, len);

		if(bindsock == -1)
		{
			perror("Bind: ");
		  	exit (-1);
		}

		//listen()
		listensock = listen(sock, 5);
		if(listensock== -1)
		{
		  	perror("Listen: ");
		  	exit(-1);
		}

		printf("Now listening on 127.0.0.1:%d\n",listeningPort );

		acceptsock = accept(sock, (struct sockaddr *)&client , &len);
		if(acceptsock == -1)
		{
		  	perror("Accept: ");
		  	exit(-1);
		}
		
		while(1)
		{
			char recvmsg[1024] = "";
			recvsock = recv(acceptsock, recvmsg, 1024, 0);
      		if (recvsock == -1)
      		{
       			perror("recvsock: ");
       			exit(-1);
      		}

      		printf("%s", recvmsg);

      		if(strcmp(recvmsg,endconv) == 0)
      		{
      			printf("Ending Connection\n");
      			close(acceptsock);
      			break;
      		}

      		char sendersmsg1[1024] = "";
			//printf("Please enter your message\n");
			//getchar();
			fgets(sendersmsg1, sizeof(sendersmsg1), stdin);
			sendsock = send(acceptsock, sendersmsg1 , strlen(sendersmsg1),0);
	    	if(sent == -1)
	    	{
	   		  	perror("No messga Sent");
	   			exit(-1);
	    	}
	    	if (strcmp(sendersmsg1,endconv) == 0)
	    	{
	    		printf("Your conversation has ended\n");
      			close(acceptsock);
	    		break;
	    	}
		}
		connection = 0;
		printf("Your conversation has ended\n");
	}




	//connect to a port and chat with a friend
	if(x == 6)
	{
		printf("-----------------------------\n");
		printf("Chat with a friend\n");
		sent = send(sock, "6" , 1,0);
	    if(sent == -1)
	    {
	   	  	perror("No messga Sent");
	   		exit(-1);
	    }
	    connection == 1;
		printf("Disconnecting from the server\n\n\n");
	    close(sock);


		char ip1[16];
		int FriendsPort;

		printf("Please enter the IP address:");
		scanf("%15s", ip1);
		printf("Please enter Port Number: ");
    	scanf("%d",&FriendsPort);


		//establish socket()
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("Error in Socket Creating");
			exit(-1);
		}

		bzero(&server, len);
		server.sin_family = AF_INET;
		server.sin_port = htons(FriendsPort);
		server.sin_addr.s_addr =inet_addr(ip1);


		//establish connect()
		connectsock = connect(sock, (struct sockaddr *) &server, sizeof(server));
		if (connectsock == -1)
		{
			perror("Error while connecting");
			exit(-1);
		}

		printf("-----------------------------\n");
		printf("Connecting to server\n");
		printf("Please enter your message\n");
		
		while(1)
		{
			char sendersmsg[1024] = "";
			
			//getchar();
			fgets(sendersmsg, sizeof(sendersmsg), stdin);
			sent = send(sock, sendersmsg , strlen(sendersmsg),0);
	    	if(sent == -1)
	    	{
	   		  	perror("No messg Sent");
	   			exit(-1);
	    	}
	    	//printf("%s\n",sendersmsg);
	    	if (strcmp(sendersmsg,endconv) == 0)
	    	{
	    		printf("Your conversation has ended\n");
	    		close(sock);
	    		break;
	    	}

	    	char recvmsg1[1024] = "";
			recvd = recv(sock, recvmsg1, 1024, 0);
      		if (recvd == -1)
      		{
       			perror("recvsock: ");
       			exit(-1);
      		}

      		printf("%s", recvmsg1);

      		if(strcmp(recvmsg1,endconv) == 0)
      		{
      			printf("Ending Connection\n");
      			close(sock);
      			break;
      		}
		}
		connection = 0;
	}

	if(x == 7)
	{
		sent = send(sock, "7" , 1,0);
	    if(sent == -1)
	    {
	   	  	perror("No messga Sent");
	   		exit(-1);
	    }
	    
		close(sock);
	}
}




//main function
int main()
{
	printf("********Welcome of Chat App*********\n");
	while(1)
	{
		int selection = 0;
	  	
	  	printf("1. Connect to the Server\n");
	  	printf("2. Get the User List\n");
	  	printf("3. Send a Message\n");
	  	printf("4. Get my messages\n");
	  	printf("5. Initiate a chat with a friend\n");
	  	printf("6. Chat with a Friend\n");
	  	printf("7. Exit\n");
	  	scanf("%d",&selection);
	  	getchar();
	    switch(selection)
	  	{
	  		case 1: 
	  			//establish connection call
	  			establishConnection(1);
	  			continue;
	  		case 2:
	  			if(connection == 0)
	  			{
	  				printf("Please connect to the server first and establish your Identity");
	  			}
	  			else
	  			{
	  				establishConnection(2);
	  				printf("Function returned\n");
	  				break;
	  			}
	  		case 3:
	  			if(connection == 0)
	  			{
	  				printf("Please connect to the server first and establish your Identity");
	  			}
	  			else
	  			{
	  				establishConnection(3);
	  				break;
	  			}
	  		case 4:
	  			if(connection == 0)
	  			{
	  				printf("Please connect to the server first and establish your Identity");
	  			}
	  			else
	  			{
	  				establishConnection(4);
	  				break;
	  			}
	  		case 5:
	  			establishConnection(5);
	  			break;
	  
	  		case 6:
	  			establishConnection(6);
	  			break;		
	  		case 7:
	  			establishConnection(7);
	  			exit(0);
	  		default:
	  			printf("Please enter a valid selection\n");


	  	}
	}
  return 0;
}