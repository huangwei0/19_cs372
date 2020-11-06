/********************************************************************************************************************
Programmer Name: Wei Huang
Program Name: ftserver.c
Description: This program is an application of API between server and client that client can send the file to server.
#             reference from  https://github.com/gregmankes/cs372-project2 and Gregmankes's project in github that posted on Nov 29,2016.
							  http://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program 
Course Name: cs372
Date: 2019/8/13
*********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>


/******************************************************************************
Function: addrinfo * create_address_ip(char*, char* )
type: struct
Describetion: creat ip adress port information and point to the main that make sure the address and port inofrmation.
return: the address list
*******************************************************************************/
struct addrinfo * create_address_ip(char * ip_addr, char * port){
	struct addrinfo hints,  *server_info;	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
  	getaddrinfo(ip_addr, port, &hints, &server_info);
	return server_info;
}


/******************************************************************************
Function: addrinfo * create_address(char*, )
type: struct
Describetion: creat port information and point to the main that make sure the port inofrmation.
return: the address list
*******************************************************************************/
struct addrinfo * create_address(char * port){
	struct addrinfo hints, *server_info;	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, port, &hints, &server_info);
	return server_info;
}

/******************************************************************************
Function: creat_socket(struct addrinfo* )
type: int
Describetion: creat the socket information from the addreinfo
return: the infromation of the socket
*******************************************************************************/
int create_socket(struct addrinfo * server_info){	
	socket((struct addrinfo *)(server_info)->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	return socket_inf;
}

/******************************************************************************
Function: creat_array(int)
type: char**
Describetion: creat an array for file
return: number of files
*******************************************************************************/
char ** create_array(int size){
	char ** array = malloc(size*sizeof(char *));
	for(int i=0; i < size; i++){
		array[i] = malloc(100*sizeof(char));
		memset(array[i],0,sizeof(array[i]));
	}
	return array;
}

/******************************************************************************
Function: check_file(char**, int,char*)
type: int 
Describetion: check the for file and put them into the arrary
return: the file array
*******************************************************************************/
int check_file(char ** files, int num, char * name){
	int list = 0;
	for (int i = 0; i < num; i++){
		if(strcmp(files[i], name) == 0){
			list = 1;
		}
	}
	return list;
}


/******************************************************************************
Function: list_files(char**)
type: int 
Describetion: put file into the arrary
return: the file array
*******************************************************************************/
int list_files(char ** files){
	int total_size = 0;
	int total_file = 0;
	struct dirent * dir;
	DIR * d = opendir(".");
	int i = 0;
	if (d){
		while ((dir = readdir(d)) != NULL){
			if (dir->d_type == DT_REG){
				file[i] = dir->d_name;
				total_size += strcpy(files[i]);
				i++;
			}
		}
		total_file = i-1;
	}
	closedir(d);
	return i;
}

/******************************************************************************
Function: recveived(int)
type: 
Describetion: the operation function between the server and client  learn from github
return: newly created socket
*******************************************************************************/
void recveived(int new_file_des){
	char port[100];
	char comm[100];
	char ip_address[100];
	char* bad = "bad";
	char* ok ="ok";

	memset(port, 0, sizeof(port));
	recv(new_file_des, port, sizeof(port)-1, 0);
	send(new_file_des, ok, strlen(ok),0);

	memset(comm,0,sizeof(comm));
	recv(new_file_des, comm, sizeof(comm)-1, 0);
	send(new_file_des, ok, strlen(ok),0);
	memset(ip_address,0,sizeof(ip_address));
	recv(new_file_des, ip_address, sizeof(ip_address)-1,0);
	printf("connection from %s\n", ip_address);

	if(strcmp(comm,"l") == 0){
		send(new_file_des, ok, strlen(ok),0);
		printf("requested from %s\n", port);
		printf("file list to %s from %s\n", ip_address, port);
		char ** files = create_array(100);
		intnum = list_files(files);
		sleep(2);
		struct addrinfo * server_info = create_address_ip(ip_address, port);
		int data = create_socket(server_info);
		char * done = "done";

		connect(data, server_info->ai_addr, server_info->ai_addrlen);
		for (int i=0; i <num; i++){
			send(data, files[i], 100,0);
		}	
		send(data, done, strlen(done),0);
		close(data);
		freeaddrinfo(server_info);
		for (int i = 0; i < 100; i++){
			free(files[i]);
		}
		free(files);
	}
	else if(strcmp(comm, "g") == 0){
		send(new_file_des, ok, strlen(ok),0);

		char name[100];
		char ** files = create_array(100);
		intnum = list_files(files);
		int file_exists = check_file(files,num, name);

		memset(name, 0, sizeof(name));
		recv(new_file_des, name, sizeof(name)-1,0);
		printf(" %s requested from %s\n", name, port);
		if(file_exists){
			printf("File found, sending %s to client\n", name);
			char * found = "File found";
			char new_name[100];
			char * end = new_name + strlen(new_name);
			sleep(2);
			struct addrinfo * server_info = create_address_ip(ip_address, port);
			int data = create_socket(server_info);
			char buff[1000];

			send(new_file_des, found, strlen(found),0);
			memset(new_name,0,sizeof(new_name));
			strcpy(new_name, "./");
			end += sprintf(end, "%s", name);

			connect(data, server_info->ai_addr, server_info->ai_addrlen);
			memset(buff, 0, sizeof(buff));
			int fd = open(new_name, O_RDONLY);

			while (1) {

				int read = read(fd, buff, sizeof(buff)-1);
				if (read == 0) 
					break;
				if (read < 0) {
					fprintf(stderr, "Error\n");
				}

		void *pointer = buff;
		while (read > 0) {
			int written = send(data, pointer, sizeof(buff),0);
			if (written < 0) {
				fprintf(stderr, "Error writing to socket\n");
				return;
			}
			read -= written;
			pointer += written;
		}

		memset(buff, 0, sizeof(buff));
	}
	memset(buff, 0, sizeof(buff));
	strcpy(buff, "__done__");
	send(data, buff, sizeof(buff),0);
	close(data);
	freeaddrinfo(server_info);
		}
		else{

			printf("File not found, sending error message to client\n");
			send(new_file_des, "File not found", 100, 0);
		}
		for (int i = 0; i < 100; i++){
		free(files[i]);
		}
		free(files);
	}
	else{
		send(new_file_des, bad, strlen("bad"), 0);
		printf("Error\n");
	}
	printf("waitting...\n");
}

/******************************************************************************
Function: main(int char*)
type: int
Describetion: put the all funtion in the main funtion that creat a complete peoject.
return: 0 that means the peoject is close.
*******************************************************************************/
int main(int argc, char *argv[]){

	struct sockaddr_storage their_addr;
    socklen_t addr_size;
	int new_file_des;

	if(argc != 2){
		fprintf(stderr, "Please enter specify the port number\n");
		exit(1);
	}
	printf("The server is ready on%s\n", argv[1]);
	struct addrinfo * server_info = create_address(argv[1]);
	int socket_inf = create_socket(server_info);
	bind(socket_inf, server_info->ai_addr, server_info->ai_addrlen);
	if(listen(socket_inf, 5) == -1){
		close(socket_inf);
		fprintf(stderr, "Error\n");
		exit(1);
	}
	while(1){
		addr_size = sizeof(their_addr);
		new_file_des = accept(socket_inf, (struct addrinfo *)&their_addr, &addr_size);
		if(new_file_des == -1){
			continue;
		}
		recveived(new_file_des);
		close(new_file_des);
	}

	freeaddrinfo(server_info);
}

