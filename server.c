#include <stdio.h>
#include <sys/types.h>  //for socket
#include <sys/socket.h> //for sockets
#include <netinet/in.h>  //for the structures
#include <string.h>   //for strings
#include <stdlib.h>
#include <errno.h> // for perror
#include <arpa/inet.h>

int main()

{

    //variables
    int sock, bindsock, listensock, acceptsock, acceptsock1;
    unsigned int len;
    int recvsock, sendsock;
    char option1;
    char option[5] = "";
    char Username[10] = "";
    char refinedUsername[10];
    char password[10] = "";
    char usernameBob[] = "bob";
    char passwordBob[] = "12345";
    char usernameAlice[] = "alice";
    char passwordAlice[] = "12345";
    char *loggedInUser;
    int userauth, passwordauth;
    char msgUsername[10];
    char msg[4096];

    //socket creation
    struct sockaddr_in server, client;
    len = sizeof(struct sockaddr_in);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Socket: ");
        exit(-1);
    }
    bzero(&server, len);
    server.sin_family = AF_INET;
    server.sin_port = htons(1234);
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
    printf("Listening on 127.0.0.1:1234\n");
    acceptsock = accept(sock, (struct sockaddr *)&client , &len);
    if(acceptsock == -1)
    {
        perror("Accept: ");
        exit(-1);
    }


    //useroption function
    void useroptions(char p)
    {

        //print user option
        printf("Option %c recieved\n", p);
        //if user option is 1 do the following operations
        if (p == '1')
        {
            printf("PLease provide your username and password\n");
            // Read username and password
            memset(Username,0,strlen(Username));
            memset(password,0,strlen(password));
            recvsock = recv(acceptsock, Username, 10, 0);
            if (recvsock == -1)
            {
                perror("recvsock: ");
                exit(-1);
            }

            recvsock = recv(acceptsock, password, 10, 0);
            if (recvsock == -1)
            {
                perror("recvsock: ");
                exit(-1);
            }


            userauth = strcmp(Username, usernameBob);



            //check username and password
            if(strcmp(Username, usernameBob) == 0)
            {
                passwordauth = strcmp(password, passwordBob);
                if (passwordauth == 0)
                {
                    printf("Logged in as Bob\n");
                    loggedInUser="bob";
                    char *welcomeMessage= "Welcome to chat App Bob\n------------------------------\n\n";
                    printf("%s\n",welcomeMessage );
                    sendsock = send(acceptsock, welcomeMessage, 50, 0);
                    if (sendsock == -1)
                    {
                        perror("sendsock: ");
                        exit(-1);
                    }
                }
                else
                {
                    printf("Please enter correct password for bob\n");
                }
            }
            else if(strcmp(Username, usernameAlice) == 0)
            {
                passwordauth = strcmp(password, passwordAlice);
                if (passwordauth == 0)
                {
                    printf("Logged in as Alice\n");
                    loggedInUser="alice";
                    char *welcomeMessage= "Welcome to chat App Alice\n------------------------------\n\n";
                    printf("%s\n",welcomeMessage );
                    sendsock = send(acceptsock, welcomeMessage, strlen(welcomeMessage), 0);
                    if (sendsock == -1)
                    {
                        perror("sendsock: ");
                        exit(-1);
                    }
                }
                else
                {
                    printf("Please enter correct password for Alice\n");
                }
            }
            else
            {
                printf("Please enter correct Username");
            }
        }


        //if user option is 2 do the following operations
        if (p == '2')
        {
            sendsock = send(acceptsock, "1. Alice\n2. Bob", strlen("1. Alice\n2. Bob"), 0);
            if (sendsock == -1)
            {
                perror("sendsock: ");
                exit(-1);
            }
            return;

        }


        //if user option is 3 do the following operations
        if (p == '3')
        {
            /* code */
            recvsock = recv(acceptsock, msgUsername, 10, 0);
            if (recvsock == -1)
            {
                perror("recvsock: ");
                exit(-1);
            }
            printf("Message Username Recieved %s", msgUsername);
            recvsock = recv(acceptsock, msg, 100, 0);
            if (recvsock == -1)
            {
                perror("recvsock: ");
                exit(-1);
            }

            printf("Message Recieved%s\n", msg);
            FILE *fp;
            fp=fopen(msgUsername, "a+");
            fprintf(fp,"%s",msg);
            fclose(fp);
            return;
        }


        //if user option is 4 do the following operations
        if (p == '4')
        {
            printf("User loggedin : %s\n", loggedInUser);
            FILE *fp1;
            fp1=fopen(loggedInUser, "r");
            char buf[1024] = "";
            fread(buf,1024,1,fp1);
            printf("%s", buf);
            sendsock = send(acceptsock, buf, strlen(buf), 0);
            if (sendsock == -1)
            {
                perror("sendsock: ");
                exit(-1);
            }
            fclose(fp1);
            return;
        }

        //if user option is 5 do the following operations
        if (p == '5')
        {
            close(acceptsock);
            printf("Close socket for %s\n", loggedInUser);
            acceptsock = accept(sock, (struct sockaddr *)&client , &len);
            if(acceptsock == -1)
            {
                perror("Accept: ");
                exit(-1);
            }
            return;

        }


        //if user option is 6 do the following operations
        if (p == '6')
        {
            close(acceptsock);
            printf("Close socket for %s\n", loggedInUser);
            acceptsock = accept(sock, (struct sockaddr *)&client , &len);
            if(acceptsock == -1)
            {
                perror("Accept: ");
                exit(-1);
            }
            return;
        }
    }


    //main loop
    while(1)
    {
        recvsock = recv(acceptsock, option, 10, 0);
        if (recvsock == -1)
        {
            perror("recvsock: ");
            exit(-1);
        }
        option1 = *option;
        if (option1 != '7' && option1 != '5' && option1 != '6')
        {
            useroptions(option1);
            continue;
        }
        else if(option1 == '5')
        {
            useroptions(option1);
            printf("Exiting\n");
            continue;
        }
        else if(option1 == '6')
        {
            useroptions(option1);
            printf("Exiting\n");
            continue;
        }
        else if(option1 == '7')
        {
            printf("Exiting\n");
            close(acceptsock);
            continue;
        }
        else
        {
            printf("Please enter a valid option\n");
            continue;
        }
    }
}

