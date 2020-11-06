/********************************************************************************************************************
Programmer Name: Wei Huang
Program Name: chatclient.c
Description: This program is a chat application of API between server and client.
#             reference from  http://docs.python.org/release/2.6.5/library/internet.html and Gregmankes's project in github that posted on nov 29,2016.
Course Name: cs372
Date: 2019/7/29
*********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

char username[10];
char servername[10];

/******************************************************************************
Function: addrinfo * create_address(char*, char*)
type: int
Describetion: creat the address and port information and point to the main that make sure there only one address and one port inofrmation.
return: the address list
*******************************************************************************/
struct addrinfo * create_address(char * input_addr, char * port){	
	struct addrinfo hints, *server_info;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(input_addr, port, &hints, &server_info);	
	return server_info;
}



/******************************************************************************
Function: creat_socket(struct addrinfo*)
type: int
Describetion: creat the socket information from the addreinfo
return: the infromation of the socket
*******************************************************************************/
int creat_socket(struct addrinfo *server_info){ 
  int socket_inf = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol); 
  	return socket_inf;
}

/******************************************************************************
Function: connect_socket(int,struct addrinfo*)
type: int
Describetion: connect the sever and client at the specific adress of addrinfo
return: the infromation of the socket and the addrinfo linked list
*******************************************************************************/
void connect_socket(int socket_inf, struct addrinfo * server_info){	
	int status = connect(socket_inf, server_info->ai_addr, server_info->ai_addrlen);
}

/******************************************************************************
Function: sendMe(int )
type: void
Describetion: send the messages to the server
*******************************************************************************/
void sendMe(int sockFD)
{
    int result;
    char output[500];
    memset(output, 0, sizeof(output));
  
    fgets(output, 500, stdin);
      printf("%s> ", username);
        scanf("%s", output);
	  output[strcspn(output, "\n")] = 0;
    result = send(sockFD, output, 500, 0);
    if(strcmp(output, "\\q") == 0)
    {
     printf("Connection closed\n");
        exit(0);
    }
}


/******************************************************************************
Function: recveived(int )
type: void
Describetion: recive the messages from the server
*******************************************************************************/
void recveived(int sockFD)
{
    int result;
    char input[500];
    memset(input, 0, sizeof(input));
    result = recv(sockFD, input, 500, 0);
    if(strcmp(input, "\\q") == 0)
    {
        printf("Connection closed\n");
        exit(0);
    }
    
    printf("%s> %s\n", servername, input);
}

/******************************************************************************
Function: chat (int, char* char*)
type: void
Describetion: make the chat room between client ans server that client send the information can show on the server, and the server can give the feedback.
return: none
*******************************************************************************/
void chat(int sockFD){
      while(1)
    {
        sendMe(sockFD);
        recveived(sockFD);
    }
}
    
/******************************************************************************
Function: main(int char*)
type: int
Describetion: put the all funtion in the main funtion that creat a complete peoject.
return: 0 that means the peoject is close.
*******************************************************************************/
int main(int argc, char *argv[]){
  	if(argc != 3){
		fprintf(stderr, "Invalid number of arguments\n");
		exit(1);
	}

    int socket;
    
    
    struct addrinfo hints;
    printf("Please enter username in 10 characters:");
    scanf("%s", username);   
    struct addrinfo *server_info = create_address(argv[1], argv[2]);
    
    socket = creat_socket(server_info);
    connect_socket(socket, server_info);
    send(socket, username, strlen(username), 0);
    recv(socket, servername, 10, 0);
    chat(socket);
    freeaddrinfo(server_info);  
    
     return 0;
}
     
     
     